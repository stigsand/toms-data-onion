#include "main_common.hpp"

int main()
{
    return main_common([] (auto & encrypted) {
        std::vector<std::byte> key(32);
        key[0] = std::byte { '=' } ^ encrypted[0];
        key[1] = std::byte { '=' } ^ encrypted[1];
        key[2] = std::byte { '[' } ^ encrypted[2];
        key[3] = std::byte { ' ' } ^ encrypted[3];
        for (int k = 3488; k < 3488 + 32; ++k)
            key[k % 32] = std::byte { '=' } ^ encrypted[k];

        std::vector<std::byte> decrypted;
        unsigned int i = 0;
        for (auto b: encrypted) {
            decrypted.push_back(b ^ key[i % 32]);
            ++i;
        }

        return decrypted;
    });
}
