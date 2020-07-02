#ifndef SKIP_TO_PAYLOAD_HPP
#define SKIP_TO_PAYLOAD_HPP

#include <istream>
#include <stdexcept>
#include <string>
#include <string_view>

inline void skip_to_payload(std::istream & is)
{
    std::string_view payload_marker = "==[ Payload ]=========";
    std::string line;
    while (getline(is, line))
        if (line.compare(0, payload_marker.size(), payload_marker) == 0)
            return;
    throw std::invalid_argument("No payload marker found");
}

#endif
