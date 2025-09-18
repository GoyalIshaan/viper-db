#include <vector>
#include <map>
#include <utility>

static constexpr int FLOAT_DIMS = 128;

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

std::vector<std::pair<float*, float>> findNSimilarVectors(const std::map<std::string, float*>& float_map, const float* query, int n_probe) {
    std::vector<std::pair<float*, float>> vector_array;
    for (const auto& [key, curr_vector] : float_map) {
        float calc_sim = cosine_similarity(query, curr_vector);
        if (vector_array.size() == n_probe) {
            float largest_sim = vector_array[n_probe - 1].second;
            vector_array[n_probe - 1].second = calc_sim < largest_sim ? calc_sim : largest_sim;
        } else {
            vector_array.emplace_back(curr_vector, calc_sim);
        };
        sort(vector_array.begin(), vector_array.end());
    }
    return vector_array;
}

