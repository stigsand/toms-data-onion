#include "main_common.hpp"
#include <bitset>

int main()
{
    return main_common([] (auto & encoded) {
        std::vector<std::byte> decoded;
        auto out = std::back_inserter(decoded);
        std::byte curr { 0 };
        std::size_t bits_set = 0;
        auto discarded = 0;
        for (auto b: encoded) {
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
        return decoded;
    });
}
