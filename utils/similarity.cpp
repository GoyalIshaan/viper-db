#include "similarity.h"

float cosine_similarity(const float* a, const float* b) {
    float dot_product = 0.0f;
    float norm_a = 0.0f;
    float norm_b = 0.0f;

    for (int i = 0; i < FLOAT_DIMS; ++i) {
        dot_product += a[i] * b[i];
        norm_a += a[i] * a[i];
        norm_b += b[i] * b[i];
    }

    return dot_product / (sqrt(norm_a) * sqrt(norm_b));
}

std::vector<std::pair<std::string, float>> findNSimilarVectors(const std::map<std::string, float*>& float_map, const float* query, const int top_k) {
    std::vector<std::pair<std::string, float>> vector_array;
    for (const auto& [key, curr_vector] : float_map) {
        float calc_sim = cosine_similarity(query, curr_vector);
        if (vector_array.size() == top_k) {
            if (calc_sim < (vector_array[top_k - 1].second)) {
                vector_array[top_k - 1].first = key;
                vector_array[top_k - 1].second = calc_sim;
            }
        } else {
            vector_array.emplace_back(key, calc_sim);
        };
        sort(vector_array.begin(), vector_array.end());
    }
    return vector_array;
}