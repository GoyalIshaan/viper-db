#ifndef SIMILARITY_H
#define SIMILARITY_H

#include "store/buffer.h"
#include <vector>
#include <utility>

static constexpr int FLOAT_DIMS = 128;

float cosine_similarity(const float* a, const float* b);

std::vector<std::pair<std::string, float>> findNSimilarVectors(const NDVectorBuffer &store, const float* query, const int top_k);

#endif