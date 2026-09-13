#include "Shift.hpp"
#include "../MIPS.hpp"
#include "../Registers.hpp"

namespace nova::ps1::shift {

static inline uint32_t Rs(uint32_t opcode) { return (opcode >> 21) & 0x1F; }
static inline uint32_t Rt(uint32_t opcode) { return (opcode >> 16) & 0x1F; }
static inline uint32_t Rd(uint32_t opcode) { return (opcode >> 11) & 0x1F; }
static inline uint32_t Shamt(uint32_t opcode) { return (opcode >> 6) & 0x1F; }

void SLL(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rd(opcode), regs.GetGPR(Rt(opcode)) << Shamt(opcode));
}

void SRL(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rd(opcode), regs.GetGPR(Rt(opcode)) >> Shamt(opcode));
}

void SRA(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rd(opcode), static_cast<uint32_t>(static_cast<int32_t>(regs.GetGPR(Rt(opcode))) >> Shamt(opcode)));
}

void SLLV(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t shift = regs.GetGPR(Rs(opcode)) & 0x1F;
    regs.SetGPR(Rd(opcode), regs.GetGPR(Rt(opcode)) << shift);
}

void SRLV(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t shift = regs.GetGPR(Rs(opcode)) & 0x1F;
    regs.SetGPR(Rd(opcode), regs.GetGPR(Rt(opcode)) >> shift);
}

void SRAV(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t shift = regs.GetGPR(Rs(opcode)) & 0x1F;
    regs.SetGPR(Rd(opcode), static_cast<uint32_t>(static_cast<int32_t>(regs.GetGPR(Rt(opcode))) >> shift));
}

void ROL(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t value = regs.GetGPR(Rt(opcode));
    uint32_t shift = Shamt(opcode);
    regs.SetGPR(Rd(opcode), (value << shift) | (value >> (32 - shift)));
}

void ROR(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t value = regs.GetGPR(Rt(opcode));
    uint32_t shift = Shamt(opcode);
    regs.SetGPR(Rd(opcode), (value >> shift) | (value << (32 - shift)));
}

}