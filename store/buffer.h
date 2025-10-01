#ifndef BUFFER_H
#define BUFFER_H

#include "store/ndvector.h"
#include <cstddef>
#include <shared_mutex>
#include <atomic>

class NDVectorBuffer {

    protected:
        struct Iterator {
            public:
                bool has_next();
                Iterator& operator++();
                NDVector* operator*();
            private :
                friend class NDVectorBuffer;
                Iterator(NDVectorBuffer *intializer, bool is_end = false);

                NDVectorBuffer *parent;
                NDVector *curr_ptr;
                NDVector * curr_end;

                uint64_t curr_source = -1;
                int curr_fd = -1;
                size_t mapped_size = 0;
                void *mapped_memory = nullptr;

                void change_source();

                void open_new_file();
                void close_file();
        };

    public :
        bool is_buffer_full = false;
        static const size_t capacity = 1024;
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

        Iterator begin();
        Iterator end();

    
    
    private :
        const char* base_name = "vectors";
        std::atomic<uint64_t> file_counter{0};
        void flush_buffer();
};

#endif