#include "main_common.hpp"
#include <bitset>

int main()
{
    return main_common([] (auto & encoded) {
        for (auto & b: encoded) {
            std::bitset<8> bs(std::to_integer<unsigned long>(b));
            bs.flip(0).flip(2).flip(4).flip(6);
            auto high = bs[7];
            bs >>= 1;
            bs.set(7, high);
            b = static_cast<std::byte>(bs.to_ulong());
        }
        return encoded;
    });
}
