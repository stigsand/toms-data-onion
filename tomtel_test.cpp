#include "tomtel.hpp"
#include <iostream>

using b = std::byte;
std::vector program {
        b { 0x50 }, b { 48 },  // MVI b <- 72
        b { 0xC2 },  // ADD a <- b
        b { 0x02 },  // OUT a
        b { 0xA8 }, b { 0x4D }, b { 0x00 }, b { 0x00 }, b { 0x00 },  // MVI32 ptr <- 0x0000004d
        b { 0x4F },  // MV a <- (ptr+c)
        b { 0x02 },  // OUT a
        b { 0x50 }, b { 0x09 },  // MVI b <- 9
        b { 0xC4 },  // XOR a <- b
        b { 0x02 },  // OUT a
        b { 0x02 },  // OUT a
        b { 0xE1 }, b { 0x01 },  // APTR 0x00000001
        b { 0x4F },  // MV a <- (ptr+c)
        b { 0x02 },  // OUT a
        b { 0xC1 },  // CMP
        b { 0x22 }, b { 0x1D }, b { 0x00 }, b { 0x00 }, b { 0x00 },  // JNZ 0x0000001d
        b { 0x48 }, b { 0x30 },  // MVI a <- 48
        b { 0x02 },  // OUT a
        b { 0x58 }, b { 0x03 },  // MVI c <- 3
        b { 0x4F },  // MV a <- (ptr+c)
        b { 0x02 },  // OUT a
        b { 0xB0 }, b { 0x29 }, b { 0x00 }, b { 0x00 }, b { 0x00 },  // MVI32 pc <- 0x00000029
    48 31  # MVI a <- 49
    02     # OUT a
    50 0C  # MVI b <- 12
    C3     # SUB a <- b
    02     # OUT a
    AA     # MV32 ptr <- lb
    57     # MV b <- (ptr+c)
    48 02  # MVI a <- 2
    C1     # CMP
    21 3A 00 00 00  # JEZ 0x0000003a
    48 32  # MVI a <- 50
    02     # OUT a
    48 77  # MVI a <- 119
    02     # OUT a
    48 6F  # MVI a <- 111
    02     # OUT a
    48 72  # MVI a <- 114
    02     # OUT a
    48 6C  # MVI a <- 108
    02     # OUT a
    48 64  # MVI a <- 100
    02     # OUT a
    48 21  # MVI a <- 33
    02     # OUT a
    01     # HALT
    65 6F 33 34 2C  # non-instruction data

int main()
try {
    for (auto b: run(program))
        std::cout << std::to_integer<char>(b); 
}
catch (std::exception const & e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
}
