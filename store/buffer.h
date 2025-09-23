#ifndef BUFFER_H
#define BUFFER_H

#include "store/ndvector.h"
#include <cstddef>

struct NDVectorBuffer {
    public :
        bool is_buffer_full = false;
        const size_t capacity = 1000;
        size_t size = 0;
        NDVector *buffer;

        NDVectorBuffer() {
            buffer = new NDVector[capacity];
        }
        ~NDVectorBuffer() {
            delete [] buffer;
        }

        void add_vector(const float *new_vector, const char *raw_id);
    
    private :
        void write_new_file();
};

#endif