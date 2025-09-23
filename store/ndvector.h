#ifndef ND_VECTOR_H
#define ND_VECTOR_H

#include <cassert>
#include <cstddef>

struct NDVector {
    char raw_id[37];
    char padding[27];
    float data[128];
} __attribute__((aligned(32)));

static_assert(sizeof(NDVector) == 576, "NDVector size mismatch");

#endif