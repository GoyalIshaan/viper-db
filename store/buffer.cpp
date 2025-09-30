#include "buffer.h"
#include "store/ndvector.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <fcntl.h>
#include <inttypes.h>
#include <unistd.h>
#include <iostream>


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


    int fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
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