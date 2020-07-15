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
    std::vector<std::byte> payload;
    ascii85_decode(
        std::istream_iterator<char>(std::cin),
        std::istream_iterator<char>(),
        std::back_inserter(payload));
    for (auto b: decoder(payload))
        std::cout << std::to_integer<char>(b);
    return 0;
}
catch (std::exception const & e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
}
