#ifndef VM_HPP
#define TOMTEL_HPP

#include "enforce.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <variant>
#include <vector>

#include <iostream>  // [stigsand]

namespace tomtel {

using u8 = std::uint8_t;
using u32 = std::uint32_t;

using output = std::vector<std::byte>;

void write(output & out, std::byte b)
{
    out.push_back(b);
}

using memory = std::vector<std::byte>;

u8 read(memory const & mem, u32 addr)
{
    enforce(addr < mem.size(), "invalid memory access");
    return std::to_integer<u8>(mem[addr]);
}

u32 read_le(memory const & mem, u32 addr)
{
    return static_cast<u32>(read(mem, addr))
            | (static_cast<u32>(read(mem, addr + 1)) << 8)
            | (static_cast<u32>(read(mem, addr + 2)) << 16)
            | (static_cast<u32>(read(mem, addr + 3)) << 24);
}

void write(memory & mem, u32 addr, u8 b)
{
    enforce(addr < mem.size(), "invalid memory access");
    mem[addr] = std::byte { b };
}

struct registers {
    std::array<u8, 6> r8 = { 0 };
    std::array<u32, 6> r32 = { 0 };
};

u8 & a(registers & regs) { return regs.r8[0]; }
u8 & b(registers & regs) { return regs.r8[1]; }
u8 & c(registers & regs) { return regs.r8[2]; }
u8 & f(registers & regs) { return regs.r8[5]; }
u32 & ptr(registers & regs) { return regs.r32[4]; }
u32 & pc(registers & regs) { return regs.r32[5]; }

struct vm {
    memory mem;
    registers regs;
    output out;    
};

// Instructions

struct add {};

void execute(add, vm & vm)
{
    a(vm.regs) += b(vm.regs);
}

struct aptr {
    u8 imm;
};

void execute(aptr instr, vm & vm)
{
    ptr(vm.regs) += instr.imm;
}

struct cmp {};

void execute(cmp, vm & vm)
{
    f(vm.regs) = a(vm.regs) == b(vm.regs) ? 0 : 0x01;
}

struct halt {};

void execute(halt, vm &) {}

struct jez {
    u32 imm;
};

void execute(jez instr, vm & vm)
{
    if (f(vm.regs) == 0)
        pc(vm.regs) = instr.imm;
}

struct jnz {
    u32 imm;
};

void execute(jnz instr, vm & vm)
{
    if (f(vm.regs) != 0)
        pc(vm.regs) = instr.imm;
}

struct mv {
    u8 dest_idx;  // 1-7
    u8 src_idx;  // 1-7
};

void execute(mv instr, vm & vm)
{
    auto b = instr.src_idx == 7
        ? read(vm.mem, ptr(vm.regs) + c(vm.regs))
        : vm.regs.r8[instr.src_idx - 1];
    if (instr.dest_idx == 7)
        write(vm.mem, ptr(vm.regs) + c(vm.regs), b);
    else
        vm.regs.r8[instr.dest_idx - 1] = b;
}

struct mv32 {
    u8 dest_idx;  // 1-6
    u8 src_idx;  // 1-6
};

void execute(mv32 instr, vm & vm)
{
    vm.regs.r32[instr.dest_idx - 1] = vm.regs.r32[instr.src_idx - 1];
}

struct mvi {
    u8 dest_idx;  // 1-7
    u8 imm;
};

void execute(mvi instr, vm & vm)
{
    if (instr.dest_idx == 7)
        write(vm.mem, ptr(vm.regs) + c(vm.regs), instr.imm);
    else
        vm.regs.r8[instr.dest_idx - 1] = instr.imm;
}

struct mvi32 {
    u8 dest_idx;  // 1 - 6
    u32 imm;
};

void execute(mvi32 instr, vm & vm)
{
    vm.regs.r32[instr.dest_idx - 1] = instr.imm;
}

struct out {};

void execute(out, vm & vm)
{
    write(vm.out, std::byte { a(vm.regs) });
}

struct sub {};

void execute(sub, vm & vm)
{
    if (b(vm.regs) > a(vm.regs))
        a(vm.regs) = 255 - b(vm.regs) + a(vm.regs);
    else
        a(vm.regs) -= b(vm.regs);
}

struct bw_xor {};

void execute(bw_xor, vm & vm)
{
    a(vm.regs) ^= b(vm.regs);
}

using instruction = std::variant<
    add, aptr, cmp, halt, jez, jnz, mv, mv32, mvi, mvi32, out, sub, bw_xor>;

instruction read_instruction(memory & mem, u32 & pc)
{
    auto b0 = read(mem, pc++);
    if (b0 == 0xC2)
        return add {};
    else if (b0 == 0xE1)
        return aptr { read(mem, pc++) };
    else if (b0 == 0xC1)
        return cmp {};
    else if (b0 == 0x01)
        return halt {};
    else if (b0 == 0x21) {
        u32 imm = read_le(mem, pc);
        pc += 4;
        return jez { imm };
    }
    else if (b0 == 0x22) {
        u32 imm = read_le(mem, pc);
        pc += 4;
        return jnz { imm };
    }
    else if ((b0 & 0b11000111) == 0b01000000) {
        u8 dest_idx = (b0 & 0b00111000) >> 3;
        enforce(dest_idx > 0, "invalid MVI {dest}");
        return mvi { dest_idx, read(mem, pc++) };
    }
    else if ((b0 & 0b11000000) == 0b01000000) {
        u8 dest_idx = (b0 & 0b00111000) >> 3;
        enforce(dest_idx > 0, "invalid MV {dest}");
        u8 src_idx = b0 & 0b00000111;
        return mv { dest_idx, src_idx };
    }
    else if ((b0 & 0b11000111) == 0b10000000) {
        u8 dest_idx = (b0 & 0b00111000) >> 3;
        enforce(dest_idx > 0 && dest_idx < 7, "invalid MVI32 {dest}");
        u32 imm = read_le(mem, pc);
        pc += 4;
        return mvi32 { dest_idx, imm };
    }
    else if ((b0 & 0b11000000) == 0b10000000) {
        u8 dest_idx = (b0 & 0b00111000) >> 3;
        enforce(dest_idx > 0 && dest_idx < 7, "invalid MV32 {dest}");
        u8 src_idx = b0 & 0b00000111;
        enforce(src_idx < 7, "invalid MV32 {src}");
        return mv32 { dest_idx, src_idx };
    }
    else if (b0 == 0x02)
        return out {};
    else if (b0 == 0xC3)
        return sub {};
    else if (b0 == 0xC4)
        return bw_xor {};
    enforce(false, "unrecognized opcode");
    return halt {};
}

bool execute_instruction(instruction const & instr, vm & vm)
{
    if (std::holds_alternative<halt>(instr))
        return false;
    std::visit(
        [&] (auto const & instr) {
            // std::cout << "[stigsand] Executing: " << typeid(instr).name() << '\n';
            execute(instr, vm); },
        instr);
    return true;
}

void run(vm & vm)
{
    while (execute_instruction(read_instruction(vm.mem, pc(vm.regs)), vm));
}

output run(memory program)
{
    vm vm;
    vm.mem = std::move(program);
    run(vm);
    return std::move(vm.out);
}

}

#endif
