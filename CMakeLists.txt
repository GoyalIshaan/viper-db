cmake_minimum_required(VERSION 3.10)
project(ViperDB)

set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf REQUIRED)
find_package(absl REQUIRED)

add_executable(test test.cpp viperdb.pb.cc)

target_link_libraries(test 
    ${Protobuf_LIBRARIES}
    absl::base
    absl::log_internal_message
    absl::log_internal_check_op
    absl::log_internal_nullguard
    absl::strings
    absl::raw_logging_internal
)

target_include_directories(test PRIVATE ${Protobuf_INCLUDE_DIRS})

add_custom_target(distClean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/Makefile
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/test
    COMMENT "Removing CMake cache and build files"
)