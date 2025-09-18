#ifndef VIPER_DB_H
#define VIPER_DB_H

#include "viperdb.grpc.pb.h"
#include "viperdb.pb.h"
#include <string>
#include <map>
#include "utils/uuid.h"
#include "utils/similarity.h"

class ViperDBService : public ViperDB::Service {
    private:
    std::map<std::string, float*> vector_map;

public:
    ViperDBService() : vector_map() {};
    ~ViperDBService() override;

    grpc::Status CreateVector(grpc::ServerContext* context, const CreateVectorRequest* request, CreateVectorResponse* response) override;
    grpc::Status SearchVector(grpc::ServerContext* context, const SearchVectorRequest* request, SearchVectorResponse* response) override;
};

#endif