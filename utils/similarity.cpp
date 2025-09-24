#include "similarity.h"
#include "store/buffer.h"
#include <iostream>

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

std::vector<std::pair<std::string, float>> findNSimilarVectors(const NDVectorBuffer& store, const float* query, const int top_k) {
    std::vector<std::pair<std::string, float>> vector_array;
    const auto data = store.buffer;
    const int data_size = store.size;
    for (int i = 0; i < data_size; ++i) {
        float calc_sim = cosine_similarity(query, data[i].data);
        if (vector_array.size() == top_k) {
            if (calc_sim < (vector_array[top_k - 1].second)) {
                vector_array[top_k - 1].first = data[i].raw_id;
                vector_array[top_k - 1].second = calc_sim;
            }
        } else {
            vector_array.emplace_back(std::string(data[i].raw_id), calc_sim);
            std::cout << "is thsib workign" << std::string(data[i].raw_id) << std::endl;
        };
        std::sort(vector_array.begin(), vector_array.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;  // Sort by similarity in descending order
        });
    }
    return vector_array;
}