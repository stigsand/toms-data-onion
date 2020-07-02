#include "skip_to_payload.hpp"
#include <exception>
#include <iostream>

int main()
try {
    skip_to_payload(std::cin);
}
catch (std::exception const & e) {
    std::cerr << e.what();
}
