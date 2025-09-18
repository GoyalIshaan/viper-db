cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(gRPC CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)

add_executable(test test.cpp viperdb.pb.cc viperdbservice.cpp
        utils/similarity.h
        utils/uuid.h
        utils/similarity.cpp
        utils/uuid.cpp)

target_link_libraries(test 
    gRPC::grpc++
)

add_custom_target(distClean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/Makefile
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/test
    COMMENT "Removing CMake cache and build files"
)