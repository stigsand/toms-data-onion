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

    std::vector<std::byte> target;
    auto out = std::back_inserter(target);
    std::byte curr { 0 };
    std::size_t bits_set = 0;
    auto discarded = 0;
    for (auto b: v) {
        std::bitset<8> bs(std::to_integer<unsigned long>(b));
        auto parity_bit = bs[0];
        auto count = bs.count() - (parity_bit ? 1 : 0);
        auto even = count % 2 == 0;
        if (even == parity_bit) {
            ++discarded;
            continue;
        }

        if (bits_set == 0) {
            curr = b;
            bits_set = 7;
        }
        else {
            auto mask = (~std::byte {0}) >> bits_set;
            curr = (curr & ~mask) | ((b >> bits_set) & mask);
            *out++ = curr;

            auto left_over = (7 - (8 - bits_set));  // 0 - 6
            curr = (b >> 1) << (8 - left_over);
            bits_set = left_over;
        }
    }

    for (auto b: target)
        std::cout << static_cast<unsigned char>(b);
}
catch (std::exception const & e) {
    std::cerr << e.what() << '\n';
}
