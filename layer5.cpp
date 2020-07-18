#include "enforce.hpp"
#include "main_common.hpp"
#include <openssl/evp.h>
#include <array>

int main()
{
    return main_common([] (auto & data) {
        enforce(data.size() >= 96);
        auto in = reinterpret_cast<unsigned char const *>(data.data());
        std::array<unsigned char, 40> key;
        {
            auto ctx = EVP_CIPHER_CTX_new();  // EVP_CIPHER_CTX_free()
            EVP_CIPHER_CTX_set_flags(ctx, EVP_CIPHER_CTX_FLAG_WRAP_ALLOW);  // must explicitly enable wrap mode!
            enforce(EVP_DecryptInit_ex(ctx, EVP_aes_256_wrap(), nullptr, in, in + 32));

            int write_len;
            enforce(EVP_CipherUpdate(ctx, key.data(), &write_len, in + 40, 40));

            int out_len = write_len;
            enforce(EVP_CipherFinal_ex(ctx, key.data() + out_len, &write_len));
            out_len += write_len;
            enforce(out_len == 32, "unexpected key length");
        }

        std::vector<std::byte> out(data.size());
        {
            auto ctx = EVP_CIPHER_CTX_new();  // EVP_CIPHER_CTX_free()
            enforce(EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), in + 80));

            int write_len;
            enforce(EVP_CipherUpdate(ctx, reinterpret_cast<unsigned char *>(out.data()), &write_len, in + 96, data.size() - 96));

            int out_len = write_len;
            enforce(EVP_CipherFinal_ex(ctx, reinterpret_cast<unsigned char *>(out.data() + out_len), &write_len));
            out_len += write_len;
            out.resize(out_len);
        }

        return out;
    });
}
