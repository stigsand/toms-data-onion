#include "main_common.hpp"
#include <arpa/inet.h>
//#include <netinet/in.h>
#include <numeric>
#include <limits>

struct ipv4_header {
    std::uint16_t a;
    std::uint16_t total_len;
    std::uint16_t c;
    std::uint16_t d;
    std::uint16_t e;
    std::uint16_t header_checksum;
    std::uint32_t src_addr;
    std::uint32_t dst_addr;
} __attribute__((__packed__));

static_assert(alignof(ipv4_header) == 1);

struct ipv4_header_cs {
    std::uint16_t ints[0];
};  //__attribute__((__packed__));

//static_assert(alignof(ipv4_header_cs) == 1);

bool check_checksum(ipv4_header const & hdr)
{
    auto & cs = *reinterpret_cast<ipv4_header_cs const *>(&hdr);
    auto sum = std::accumulate(cs.ints, cs.ints + 10, std::uint32_t { 0 },
        [] (auto s, auto n) { return s + ntohs(n); });
    while (sum > std::numeric_limits<std::uint16_t>::max())
        sum = (sum & 0xFFFF) + (sum >> 16);
    std::uint16_t t = ~sum;
    //std::cout << "IP sum: " << t << '\n';
    return t == 0;
}

bool ip_addr_compare(std::uint32_t addr, std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d)
{
    return ((addr & 0xFF000000) >> 24) == a
        && ((addr & 0x00FF0000) >> 16) == b
        && ((addr & 0x0000FF00) >> 8) == c
        && (addr & 0x000000FF) == d;
}

struct udp_header {
    std::uint16_t src_port;
    std::uint16_t dst_port;
    std::uint16_t len;
    std::uint16_t checksum;
} __attribute__((__packed__));

static_assert(alignof(udp_header) == 1);


bool check_checksum(ipv4_header const & ip_hdr, udp_header const & udp_hdr, std::byte * udp_data, std::size_t udp_data_size)
{
    // See https://en.wikipedia.org/wiki/User_Datagram_Protocol#IPv4_pseudo_header

    //std::cout << "udp_data_size: " << udp_data_size << " - udp_hdr.len: " << udp_hdr.len << '\n';

    std::uint32_t sum = 0;

    // IPv4 pseudo header
    sum += ip_hdr.src_addr >> 16;
    sum += ip_hdr.src_addr & 0xFFFF;
    sum += ip_hdr.dst_addr >> 16;
    sum += ip_hdr.dst_addr & 0xFFFF;
    sum += ntohs(ip_hdr.e) & 0xFF;
    sum += udp_hdr.len;

    // UDP header
    sum += udp_hdr.src_port;
    sum += udp_hdr.dst_port;
    sum += udp_hdr.len;
    sum += udp_hdr.checksum;

    // UDP data
    std::uint16_t const * p = reinterpret_cast<std::uint16_t const *>(udp_data);
    for (int i = 0; i < udp_data_size/2; ++i, ++p)
        sum += ntohs(*p);
    if (udp_data_size % 2 == 1)
        sum += (std::to_integer<std::uint16_t>(udp_data[udp_data_size - 1]) << 8);

    while (sum > std::numeric_limits<std::uint16_t>::max())
        sum = (sum & 0xFFFF) + (sum >> 16);
    std::uint16_t t = ~sum;
    // std::cout << "UDP sum: " << t << '\n';
    return t == 0;
}

void print(ipv4_header const & hdr)
{
    std::cout << "Total length: " << hdr.total_len << '\n';
    /*std::cout << "Header checksum: " << hdr.header_checksum << '\n';*/

    std::cout << "Source address: "
        << ((hdr.src_addr & 0xFF000000) >> 24) << '.'
        << ((hdr.src_addr & 0x00FF0000) >> 16) << '.'
        << ((hdr.src_addr & 0x0000FF00) >> 8) << '.'
        << (hdr.src_addr & 0x000000FF) << '\n';
    std::cout << "Destination address: "
        << ((hdr.dst_addr & 0xFF000000) >> 24) << '.'
        << ((hdr.dst_addr & 0x00FF0000) >> 16) << '.'
        << ((hdr.dst_addr & 0x0000FF00) >> 8) << '.'
        << (hdr.dst_addr & 0x000000FF) << '\n';
}

void print(udp_header const & hdr)
{
    std::cout << "Source port: " << hdr.src_port << '\n';
    std::cout << "Destination port: " << hdr.dst_port << '\n';
    std::cout << "Length: " << hdr.len << '\n';
    std::cout << "Checksum: " << hdr.checksum << '\n';
}

int main()
{
    return main_common([] (auto & data) {
        std::vector<std::byte> out;
        std::vector<std::byte>::size_type i = 0;
        while (true) {
            if (i == data.size())
                break;
            if (data.size() < i + 20)
                throw std::invalid_argument("IP header too short");
            ipv4_header & ip_hdr = *reinterpret_cast<ipv4_header *>(data.data() + i);

            auto ip_checksum_ok = check_checksum(ip_hdr);

            ip_hdr.total_len = ntohs(ip_hdr.total_len);
            ip_hdr.src_addr = ntohl(ip_hdr.src_addr);
            ip_hdr.dst_addr = ntohl(ip_hdr.dst_addr);
            //print(ip_hdr);
            if (data.size() < i + ip_hdr.total_len)
                throw std::invalid_argument("IP payload too short");
            if (ip_hdr.total_len < 20 + 8)
                throw std::invalid_argument("Total length too short for UDP header");
            udp_header & udp_hdr = *reinterpret_cast<udp_header *>(data.data() + i + 20);
            udp_hdr.src_port = ntohs(udp_hdr.src_port);
            udp_hdr.dst_port = ntohs(udp_hdr.dst_port);
            udp_hdr.len = ntohs(udp_hdr.len);
            udp_hdr.checksum = ntohs(udp_hdr.checksum);
            //print(udp_hdr);

            if (ip_hdr.total_len != 20 + udp_hdr.len)
                throw std::invalid_argument("UDP packet doesn't fit in IP payload");

            auto udp_checksum_ok = check_checksum(ip_hdr, udp_hdr, data.data() + i + 20 + 8, udp_hdr.len - 8);

            if (ip_addr_compare(ip_hdr.src_addr, 10, 1, 1, 10)
                && ip_addr_compare(ip_hdr.dst_addr, 10, 1, 1, 200)
                && udp_hdr.dst_port == 42069
                && ip_checksum_ok
                && udp_checksum_ok) {
                out.insert(out.end(), data.begin() + i + 20 + 8, data.begin() + i + ip_hdr.total_len);
            }
            else {
                /*
                std::cout << "========== Packet discarded ============\n";
                auto print_ip_hdr = false;
                auto print_udp_hdr = false;
                if (!ip_addr_compare(ip_hdr.src_addr, 10, 1, 1, 10)) {
                    std::cout << "Wrong source address\n";
                    print_ip_hdr = true;
                }
                if (!ip_addr_compare(ip_hdr.dst_addr, 10, 1, 1, 200)) {
                    std::cout << "Wrong destination address\n";
                    print_ip_hdr = true;
                }
                if (udp_hdr.dst_port != 42069) {
                    std::cout << "Wrong destination port\n";
                    print_udp_hdr = true;
                }
                if (!ip_checksum_ok)
                    std::cout << "IP checksum invalid\n";
                if (!udp_checksum_ok)
                    std::cout << "UDP checksum invalid\n";
                if (print_ip_hdr)
                    print(ip_hdr);
                if (print_udp_hdr)
                    print(udp_hdr);
                std::cout << '\n';
                */
            }

            i += ip_hdr.total_len;
        }

        return out;
    });
}
