#include "viperdb.pb.h"
#include <iostream>

int main() {
    CreateVectorRequest request;
    request.add_vector(0.1f);
    request.add_vector(0.2f);
    request.add_vector(0.3f);
    
    std::cout << "Vector size: " << request.vector_size() << std::endl;
    std::cout << "First element: " << request.vector(0) << std::endl;
    
    return 0;
}