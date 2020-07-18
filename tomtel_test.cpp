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
        b { 0x48 }, b { 0x31 },  // MVI a <- 49
        b { 0x02 },  // OUT a
        b { 0x50 }, b { 0x0C },  // MVI b <- 12
        b { 0xC3 },  // SUB a <- b
        b { 0x02 },  // OUT a
        b { 0xAA },  // MV32 ptr <- lb
        b { 0x57 },  // MV b0x <- (ptr+c)
        b { 0x48 }, b { 0x02 },  // MVI a <- 2
        b { 0xC1 },  // CMP
        b { 0x21 }, b { 0x3A }, b { 0x00 }, b { 0x00 }, b { 0x00 },  // JEZ 0x0000003a
        b { 0x48 }, b { 0x32 },  // MVI a <- 50
        b { 0x02 },  // OUT a
        b { 0x48 }, b { 0x77 },  // MVI a <- 119
        b { 0x02 },  // OUT a
        b { 0x48 }, b { 0x6F },  // MVI a <- 111
        b { 0x02 },  // OUT a
        b { 0x48 }, b { 0x72 },  // MVI a <- 114
        b { 0x02 },  // OUT a
        b { 0x48 }, b { 0x6C },  // MVI a <- 108
        b { 0x02 },  // OUT a
        b { 0x48 }, b { 0x64 },  // MVI a <- 100
        b { 0x02 },  // OUT a
        b { 0x48 }, b { 0x21 },  // MVI a <- 33
        b { 0x02 },  // OUT a
        b { 0x01 },  // HALT
        b { 0x65 }, b { 0x6F }, b { 0x33 }, b { 0x34 }, b { 0x2C }  // non-instruction data
};

int main()
try {
    for (auto b: run(program))
        std::cout << std::to_integer<char>(b); 
}
catch (std::exception const & e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
}
