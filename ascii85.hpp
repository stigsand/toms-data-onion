#ifndef ASCII85_HPP
#define ASCII85_HPP

#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

#include <iostream>

template <typename T, typename InIt, typename OutIt>
OutIt ascii85_decode(InIt begin, InIt end, OutIt out)
{
    // Eat any whitespace and start delimiter "<"
    while (begin != end && std::isspace(static_cast<unsigned char>(*begin)))
        ++begin;
    if (begin == end || *begin++ != '<' || begin == end || *begin++ != '~')
        throw std::invalid_argument("missing/invalid start delimiter");

    std::uint8_t i = 0;
    std::uint8_t padded = 0;
    std::uint32_t n = 0;
    while (true) {
        char c;
        if (begin != end && padded == 0 && (c = *begin++) != '~') {
            if (std::isspace(static_cast<unsigned char>(c)))
                continue;
            if (c == 'z') {
                if (i > 0U)
                    throw std::invalid_argument("'z' in middle of group");
            }
            if (c < 33 || c > 117)
                throw std::invalid_argument("invalid character");
        }
        else {
            if (i == 0U)
                break;
            if (i == 1U)
                throw std::invalid_argument("unexpected padding");
            c = 'u';
            ++padded;
        }
        if (c != 'z') {
            n *= 85;
            n += c - 33;
            i = (i < 4U) ? i + 1 : 0;
        }
        if (i == 0U)  {
            assert(padded <= 3);
            *out++ = T(/*std::byte*/n >> 24);
            if (padded == 3)
                break;
            *out++ = T(/*std::byte*/n >> 16);
            if (padded == 2)
                break;
            *out++ = T(/*std::byte*/n >> 8);
            if (padded == 1)
                break;
            *out++ = T(/*std::byte*/n);
            n = 0;
        }
    }
    return out;
}

#endif
