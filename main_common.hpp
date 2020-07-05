#include "ascii85.hpp"
#include "skip_to_payload.hpp"
#include <exception>
#include <iostream>
#include <iterator>
#include <vector>

template <class Fun>
int main_common(Fun && decoder)
try {
    skip_to_payload(std::cin);
    std::vector<std::byte> encoded;
    ascii85_decode(
        std::istream_iterator<char>(std::cin),
        std::istream_iterator<char>(),
        std::back_inserter(encoded));
    auto decoded = decoder(encoded);
    for (auto b: decoded)
        std::cout << static_cast<char>(b);
    return 0;
}
catch (std::exception const & e) {
    std::cerr << e.what() << '\n';
    return 1;
}
