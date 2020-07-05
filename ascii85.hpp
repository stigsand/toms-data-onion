#ifndef ASCII85_HPP
#define ASCII85_HPP

#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>

template <typename InIt>
char checked_bump(InIt & begin, InIt end)
{
    if (begin == end)
        throw std::invalid_argument("unexpected end of input");
    return *begin++;
}

template <typename InIt, typename OutIt>
OutIt ascii85_decode(InIt begin, InIt end, OutIt out)
{
    // Eat any whitespace and start delimiter "<"
    while (begin != end && std::isspace(static_cast<unsigned char>(*begin)))
        ++begin;
    if (checked_bump(begin, end) != '<' || checked_bump(begin, end) != '~')
        throw std::invalid_argument("missing/invalid start delimiter");

    std::uint8_t i = 0;
    std::uint8_t padding = 0;
    std::uint32_t n = 0;
    while (true) {
        char c;
        if (padding == 0 && (c = checked_bump(begin, end)) != '~') {
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
            ++padding;
        }
        if (c != 'z') {
            if (n > std::numeric_limits<std::uint32_t>::max() / 85)
                throw std::invalid_argument("overflow in group");
            n *= 85;
            n += c - 33;
            if (n < (c - 33))
                throw std::invalid_argument("overflow in group");
            i = (i < 4U) ? i + 1 : 0;
        }
        if (i == 0U)  {
            assert(padding <= 3);
            *out++ = std::byte(n >> 24);
            if (padding == 3)
                break;
            *out++ = std::byte(n >> 16);
            if (padding == 2)
                break;
            *out++ = std::byte(n >> 8);
            if (padding == 1)
                break;
            *out++ = std::byte(n);
            n = 0;
        }
    }

    if (checked_bump(begin, end) != '>')
        throw std::invalid_argument("missing/invalid end delimiter");

    return out;
}

#endif
