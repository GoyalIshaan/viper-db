#include "buffer.h"
#include "store/ndvector.h"
#include <climits>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <fcntl.h>
#include <inttypes.h>
#include <unistd.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h> 

void NDVectorBuffer::add_vector(const float *new_vector, const char *raw_id) {
    std::unique_lock lock(mutex);

    NDVector vec;
    memcpy(vec.data, new_vector, sizeof(float) * 128);
    memcpy(vec.raw_id, raw_id, sizeof(char) * 37);

    buffer[size] = vec;
    size++;

    if (size >= capacity) {
        is_buffer_full = true;
        lock.unlock();
        flush_buffer();
    }
}



void NDVectorBuffer::flush_buffer() {
    if  (size == 0) return;

    char filename[64];
    const uint64_t current_file = file_counter.fetch_add(1);
    snprintf(filename, 64, "%s_%06" PRIu64 ".dat", base_name, current_file);


    int fd = open(filename, O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
        std::cerr << "[ERROR] Failed to open " << filename << "\n";
        return;
    };

    size_t bytes_to_write = sizeof(NDVector) * size;
    ssize_t written = write(fd, buffer, bytes_to_write);

    if (written != (ssize_t) bytes_to_write) {
        std::cerr << "[ERROR] Partial write! Expected " << bytes_to_write 
                << " but wrote " << written << "\n";
        close(fd);
        return;
    }

    if (fsync(fd) != 0) {
        std::cerr << "[ERROR] fsync failed!\n";
        close(fd);
        return;
    }

    close(fd);


    size = 0;
    is_buffer_full = false;
}

NDVectorBuffer::Iterator begin() {
    return NDVectorBuffer::Iterator(*this, false);
}

NDVectorBuffer::Iterator::Iterator(NDVectorBuffer *intializer, bool is_end) : parent(intializer){
    if (is_end) {
        curr_ptr = nullptr;
        curr_end = nullptr;
        curr_source = INT_MAX;
    } else {
        curr_ptr = parent -> buffer;
        curr_end = &(parent -> buffer[parent -> size]);
    }
}

NDVectorBuffer::Iterator& NDVectorBuffer::Iterator::operator++(){
    curr_ptr++;
    if (curr_ptr == curr_end) {
        change_source();
    }

    return (*this);
}

NDVector *NDVectorBuffer::Iterator::operator*() {
    return curr_ptr;
}

void NDVectorBuffer::Iterator::change_source() {
    if (++curr_source >= parent -> file_counter.fetch_add(0)) return;

    open_new_file();
}

void NDVectorBuffer::Iterator::open_new_file(){
    char file_name[64];
    snprintf(file_name, 64, "%s_%06" PRIu64 ".dat", parent -> base_name,  curr_source);

    curr_fd = open(file_name, O_RDONLY);
    if (curr_fd == -1) {
        curr_ptr = nullptr;
        curr_end = nullptr;
        curr_source = INT_MAX;
        return;
    }

    struct stat sb;
    if (fstat(curr_fd, &sb) == -1) {
        perror("fstat");
        close(curr_fd);
        return;
    }

    mapped_size = sb.st_size;

    mapped_memory = mmap(
        nullptr,           // addr: let OS choose address
        mapped_size,       // length: map entire file
        PROT_READ,         // prot: read-only access
        MAP_PRIVATE,       // flags: private copy-on-write (though we won't write)
        curr_fd,        // fd: file descriptor
        0                  // offset: start from beginning
    );
    
    if (mapped_memory == MAP_FAILED) {
        perror("mmap");
        close(curr_fd);
        return;
    }

    curr_ptr = static_cast<NDVector*>(mapped_memory);
    size_t num_vectors = mapped_size / sizeof(NDVector);
    curr_end = curr_ptr + num_vectors;

    madvise(mapped_memory, mapped_size, MADV_SEQUENTIAL);
}

void NDVectorBuffer::Iterator::close_file() {
    if (mapped_memory != nullptr && mapped_memory != MAP_FAILED) {
        munmap(mapped_memory, mapped_size);
    }
    
    if (curr_fd != -1) {
        close(curr_fd);
        curr_fd = -1;
    }
}