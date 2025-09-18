#ifndef SIMILARITY_H
#define SIMILARITY_H

#include <vector>
#include <map>
#include <utility>

static constexpr int FLOAT_DIMS = 128;

float cosine_similarity(const float* a, const float* b);

std::vector<std::pair<std::string, float>> findNSimilarVectors(const std::map<std::string, float*>& float_map, const float* query, const int top_k);

#endif