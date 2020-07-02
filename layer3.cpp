#include "ascii85.hpp"
#include "skip_to_payload.hpp"
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
        
    std::vector<std::byte> key(32);

    key[0] = std::byte {'='} ^ v[0];
    key[1] = std::byte {'='} ^ v[1];
    key[2] = std::byte {'['} ^ v[2];
    key[3] = std::byte {' ' } ^ v[3];

    for (int k = 3488; k < 3488 + 32; ++k)
        key[k % 32] = std::byte { '=' } ^ v[k];

    std::vector<std::byte> decrypted;
    unsigned int i = 0;
    for (auto b: v) {
        decrypted.push_back(b ^ key[i % 32]);
        ++i;
    }


    std::vector<std::byte> target;
    int j = 0;
    for (auto b: decrypted) {
        std::cout << static_cast<unsigned char>(b);
    }
}
catch (std::exception const & e) {
    std::cerr << e.what() << '\n';
}
