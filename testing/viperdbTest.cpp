#include "viperdbTest.h"
#include "viperdb.pb.h"
#include "viperdbservice.h"
#include <grpcpp/support/status.h>
#include <memory>
#include <random>

CreateVectorRequest ViperDBTest::createRandomVector() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 100);
    CreateVectorRequest newRequest;
    std::uniform_real_distribution<float> distrib_float(0.0f, 5.0f);

    for (int i = 0; i < 128; ++i) {
        newRequest.add_vector(distrib_float(gen));
    }

    return newRequest;
}


TEST_F(ViperDBTest, InsertValidator) {
    CreateVectorRequest newRequest = createRandomVector();

    grpc::ServerContext context;
    CreateVectorResponse response;
    grpc::Status result = service -> CreateVector(&context, &newRequest, &response);

    EXPECT_TRUE(result.ok()) << "gRPC call failed" << result.error_message();
    EXPECT_NE(response.id(), "") << "Id should not be an empty string";

    std::cout << "Vector string id: " << response.id() << std::endl;
}