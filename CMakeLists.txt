cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(gRPC CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(UUID REQUIRED uuid)

add_executable(test test.cpp viperdb.pb.cc viperdbservice.cpp
        utils/cosineSimilairty.cpp)

target_link_libraries(test 
    gRPC::grpc++
    ${UUID_LIBRARIES}
)

target_include_directories(test PRIVATE ${UUID_INCLUDE_DIRS})
target_compile_options(test PRIVATE ${UUID_CFLAGS_OTHER})

add_custom_target(distClean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/Makefile
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/test
    COMMENT "Removing CMake cache and build files"
)