#include "viperdbTest.h"
#include "viperdb.pb.h"
#include "viperdbservice.h"
#include <grpcpp/support/status.h>
#include <gtest/gtest.h>
#include <memory>
#include <random>
#include <string>
#include "../store/buffer.h"

extern NDVectorBuffer buffer;

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

void ViperDBTest::SetUp(){buffer.size = 0; };


TEST_F(ViperDBTest, InsertValidator) {
    CreateVectorRequest newRequest = createRandomVector();

    grpc::ServerContext context;
    CreateVectorResponse response;
    grpc::Status result = service -> CreateVector(&context, &newRequest, &response);

    EXPECT_TRUE(result.ok()) << "gRPC call failed" << result.error_message();
    EXPECT_NE(response.id(), "") << "Id should not be an empty string";

    std::cout << "Vector string id: " << response.id() << std::endl;
}

TEST_F(ViperDBTest, SearchValidator) {
    CreateVectorRequest create_vector_request = createRandomVector();
    grpc::ServerContext context;
    CreateVectorResponse response;
    grpc::Status result = service -> CreateVector(&context, &create_vector_request, &response);

    EXPECT_TRUE(result.ok()) << "gRPC call failed" << result.error_message();

    const std::string inserted_id = response.id();
    SearchVectorRequest search_request;

    for(int i = 0; i < create_vector_request.vector_size(); ++i) {
        search_request.add_query(create_vector_request.vector(i));
    }

    search_request.set_top_k(1);

    SearchVectorResponse search_response;

    result = service -> SearchVector(&context, &search_request, &search_response);

    EXPECT_TRUE(result.ok()) << "gRPC call failed" << result.error_message();

    const std::string retrieved_id = search_response.results(0).id();
    EXPECT_EQ(retrieved_id, inserted_id)
    << "id's are different";

    std::cout << "Inserted id - " << inserted_id << " , retrieved id - " << retrieved_id << std::endl;
}