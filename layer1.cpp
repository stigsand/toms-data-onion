#include "ascii85.hpp"
#include "skip_to_payload.hpp"
#include <bitset>
#include <exception>
#include <iostream>
#include <iterator>
#include <vector>

int main()
try {
    skip_to_payload(std::cin);
    std::vector<std::byte> v;
    ascii85_decode<std::byte>(
        std::istream_iterator<char>(std::cin),
        std::istream_iterator<char>(),
        std::back_inserter(v));
    for (auto b: v) {
        std::bitset<8> bs(std::to_integer<unsigned long>(b));
        bs.flip(0).flip(2).flip(4).flip(6);
        auto high = bs[7];
        bs >>= 1;
        bs.set(7, high);
        std::cout << static_cast<unsigned char>(bs.to_ulong());
    }
}
catch (std::exception const & e) {
    std::cerr << e.what() << '\n';
}
