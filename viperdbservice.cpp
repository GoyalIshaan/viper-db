#include "viperdbservice.h"

grpc::Status ViperDBService::CreateVector(grpc::ServerContext* context, const CreateVectorRequest* request, CreateVectorResponse* response) {
    if (request -> vector_size() != 128) return grpc::Status::CANCELLED;
    std::string id = generateUUID();
    auto* newVector = new float[request->vector_size()];
    for (int i = 0; i < request->vector_size(); i++) {
        newVector[i] = request->vector(i);
    }
    vector_map[id] = newVector;
    response->set_id(id);

    return grpc::Status::OK;
}

grpc::Status ViperDBService::SearchVector(grpc::ServerContext* context, const SearchVectorRequest* request, SearchVectorResponse* response) {
    auto* queryVec = new float[request->query_size()];
    for (int i = 0; i < request->query_size(); i++) {
        queryVec[i] = request->query(i);
    }
    std::vector<std::pair<std::string, float>> result = findNSimilarVectors(vector_map, queryVec, request -> top_k());
    for (const auto& [id, sim] : result) {
        SearchVectorResult new_result;
        new_result.set_id(id);
        new_result.set_similarity(sim);
        *(response -> add_results()) = new_result;
    }

    delete[] queryVec;
     return grpc::Status::OK;
}

ViperDBService::~ViperDBService() {
    for (const auto& [key, value] : vector_map) {
        delete value;
    };
}