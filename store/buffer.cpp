#include "buffer.h"
#include "store/ndvector.h"
#include <cstring>
void NDVectorBuffer::add_vector(const float *new_vector, const char *raw_id) {
    NDVector vec;
    memcpy(vec.data, new_vector, sizeof(float) * 128);
    memcpy(vec.raw_id, raw_id, sizeof(char) * 37);

    size++;
}