# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

ViperDB is a C++ vector database service that provides gRPC-based APIs for storing, searching, and retrieving high-dimensional vectors (fixed at 128 dimensions). It implements cosine similarity search for finding the most similar vectors to a query vector.

## Build System & Commands

This project uses CMake with the following key commands:

```bash
# Build the project
mkdir -p build && cd build
cmake ..
make

# Run all tests
ctest

# Run specific test executables
./viperdb_tests           # Similarity algorithm tests
./viperdb_service_tests   # gRPC service tests

# Clean build artifacts
make distClean
```

## Architecture

### Core Components

- **viperdbservice.cpp/h**: Main gRPC service implementation that handles vector CRUD operations
- **viperdb.proto**: Protocol buffer definitions for gRPC service interface
- **utils/similarity.cpp/h**: Vector similarity algorithms (cosine similarity) and search logic
- **utils/uuid.cpp/h**: UUID generation utilities for vector IDs

### Service Operations

The gRPC service provides three main operations:
1. `CreateVector`: Stores a 128-dimensional vector and returns a UUID
2. `SearchVector`: Finds top-K most similar vectors using cosine similarity
3. `GetVectorById`: Retrieves a stored vector by its UUID

### Data Storage

- Vectors are stored in-memory using `std::map<std::string, float*>`
- Each vector is exactly 128 dimensions (enforced at API level)
- Memory is manually managed with `new[]`/`delete[]`

### Testing Structure

- **testing/similarityTest.cpp**: Tests for vector similarity algorithms
- **testing/viperdbTest.cpp/h**: Integration tests for gRPC service operations
- Uses Google Test framework via FetchContent

## Key Implementation Details

- Vector dimensions are hardcoded to 128 (`FLOAT_DIMS` constant)
- Cosine similarity is used for vector search operations
- Search results are sorted by similarity score
- UUID generation provides unique identifiers for stored vectors
- gRPC service runs with reflection enabled for debugging

## Development Notes

- The project requires gRPC and protobuf dependencies
- Generated protobuf files (*.pb.cc, *.pb.h) are committed to repo
- CMake exports compile commands for IDE integration
- Memory management follows RAII principles in the service destructor