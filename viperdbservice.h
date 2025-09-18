#include "viperdb.grpc.pb.h"
#include "viperdb.pb.h"
#include <uuid/uuid.h>
#include <string>
#include <map>
#include <vector>

class ViperDBService : public ViperDB::Service {
    private:
    std::map<std::string, float*> vector_map;

public:
    ViperDBService() : vector_map() {};
    ~ViperDBService();

    grpc::Status CreateVector(grpc::ServerContext* context, const CreateVectorRequest* request, CreateVectorResponse* response) override;
    grpc::Status SearchVector(grpc::ServerContext* context, const SearchVectorRequest* request, SearchVectorResponse* response) override;

private:
    std::string generateUUID();
};