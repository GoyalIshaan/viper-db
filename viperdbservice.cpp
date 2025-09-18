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
    std::string id = generateUUID();
    std::vector<float> newVector = request->vector();
    vector_map[id] = newVector;    
    response->set_id(id);
    return grpc::Status::OK;
}

grpc::Status ViperDBService::SearchVector(grpc::ServerContext* context, const SearchVectorRequest* request, SearchVectorResponse* response) {