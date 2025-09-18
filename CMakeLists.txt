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

# Create executable with all source files
add_executable(test 
    test.cpp 
    viperdb.pb.cc 
    viperdb.grpc.pb.cc          # You were missing this!
    viperdbservice.cpp
    utils/similarity.cpp
    utils/uuid.cpp
)

# Link all required libraries
target_link_libraries(test
    gRPC::grpc++
    gRPC::grpc++_reflection     # Often needed for reflection
    protobuf::libprotobuf       # You were missing protobuf linking!
)

# Include directories for headers
target_include_directories(test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}     # For your headers
    ${CMAKE_CURRENT_BINARY_DIR}     # For generated .pb.h files
)

add_custom_target(distClean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/Makefile
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_BINARY_DIR}/test  # Fixed: was missing CMAKE_BINARY_DIR
    COMMENT "Removing CMake cache and build files"
)