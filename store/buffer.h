#ifndef BUFFER_H
#define BUFFER_H

#include "store/ndvector.h"
#include <cstddef>
#include <shared_mutex>
#include <atomic>

struct NDVectorBuffer {

    public :
        bool is_buffer_full = false;
        const size_t capacity = 1000;
        size_t size = 0;
        NDVector *buffer;

        std::shared_mutex mutex;

        NDVectorBuffer() {
            buffer = new NDVector[capacity];
        }
        ~NDVectorBuffer() {
            delete [] buffer;
        }

        void add_vector(const float *new_vector, const char *raw_id);
    
    private :
        const char* base_name = "vectors";
        std::atomic<uint64_t> file_counter{0};
        void flush_buffer();
};

#endif