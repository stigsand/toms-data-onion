#include "ascii85.hpp"
#include "skip_to_payload.hpp"
#include <exception>
#include <iostream>
#include <iterator>

int main()
try {
    skip_to_payload(std::cin);
    ascii85_decode<char>(
        std::istream_iterator<char>(std::cin),
        std::istream_iterator<char>(),
        std::ostream_iterator<char>(std::cout));
}
catch (std::exception const & e) {
    std::cerr << e.what() << '\n';
}
