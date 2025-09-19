cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
project(ViperDB)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable compile commands for VS Code IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find required packages
find_package(gRPC CONFIG REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

# Create a library with your core functionality (excluding main)
add_library(viperdb_core
    viperdb.pb.cc
    viperdb.grpc.pb.cc
    viperdbservice.cpp
    utils/similarity.cpp
    utils/uuid.cpp
)

target_link_libraries(viperdb_core
    gRPC::grpc++
    gRPC::grpc++_reflection
    protobuf::libprotobuf
)

target_include_directories(viperdb_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Test executable
add_executable(viperdb_tests testing/similarityTest.cpp)

# THIS IS THE FIX: Link the executable to the GTest libraries
# The exact names might be gtest, gmock, etc., depending on your setup.
# This is the modern CMake way.
# 'viperdb_tests' should be the name of your test executable
target_link_libraries(viperdb_tests PRIVATE GTest::gtest_main viperdb_core)

add_executable(viperdb_service_tests testing/viperdbTest.cpp testing/viperdbTest.h)
target_link_libraries(viperdb_service_tests PRIVATE GTest::gtest_main viperdb_core)


# Enable testing
enable_testing()
add_test(NAME ViperDBTests COMMAND viperdb_tests)

# Your existing clean target
add_custom_target(distClean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/Makefile
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMENT "Removing CMake cache and build files"
)