#include "viperdbservice.h"
#include <sstream>
#include <iomanip>


std::string ViperDBService::generateUUID() {
    uuid_t uuid;
    char uuid_str[37];
    
    uuid_generate(uuid);
    uuid_unparse(uuid, uuid_str);
    
    return std::string(uuid_str);
}

grpc::Status ViperDBService::CreateVector(grpc::ServerContext* context, const CreateVectorRequest* request, CreateVectorResponse* response) {
    if (request -> vector_size() >=128) return grpc::Status::CANCELLED;
    std::string id = generateUUID();
    float* newVector = new float[request->vector_size()];
    memcpy(newVector, &request -> vector(), sizeof(float) * request->vector_size());
    vector_map[id] = newVector;
    response->set_id(id);
    return grpc::Status::OK;
}

grpc::Status ViperDBService::SearchVector(grpc::ServerContext* context, const SearchVectorRequest* request, SearchVectorResponse* response) {
     return grpc::Status::OK;
}