#ifndef VM_HPP
#define TOMTEL_HPP

#include <cstddef>
#include <cstdint>
#include <variant>
#include <vector>

using u8 = std::uint8_t;
using u32 = std::uint32_t;

// Memory

class memory {
public:
    // operator[]

private:
    std::vector<std::byte> bytes;
};

// Registers

struct registers {
    u8 a, b, c, d = 0, e = 0, f = 0;
    u32 la, lb, lc, ld = 0, ptr, pc = 0;

};

// Output

using output = std::vector<std::byte>;

// VM

struct tomtel {
    memory mem;
    registers regs;
    output out;    
};

// Instructions

struct halt {};

bool execute(halt const &, tomtel &)
{
    return false;
}

using instruction = std::variant<halt>;

instruction parse(memory & mem, u32 & pc)
{
    return halt {};
}

bool execute(instruction const & instr, tomtel & vm)
{
    return std::visit(
        [&] (auto const & instr) { return execute(instr, vm); },
        instr);
}

void run(tomtel & vm)
{
    while (execute(parse(vm.mem, vm.regs.pc), vm));
}

output run(std::vector<std::byte> program)
{
    tomtel vm;
    // vm.mem == std::move(program);
    run(vm);
    return std::move(vm.out);
}

#endif
