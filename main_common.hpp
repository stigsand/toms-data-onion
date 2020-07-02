#include "ascii85.hpp"
#include "skip_to_payload.hpp"
#include <exception>
#include <iostream>
#include <iterator>
#include <vector>

template <Fun>
int main_common(Fun& f)
try {
    skip_to_payload(std::cin);
    std::vector<std::byte> v;
    ascii85_decode<std::byte>(
        std::istream_iterator<char>(std::cin),
        std::istream_iterator<char>(),
        std::back_inserter(v));
    fun(v, std::cout);
    return 0;
}
catch (std::exception const & e) {
    std::cerr << e.what() << '\n';
    return 1;
}
