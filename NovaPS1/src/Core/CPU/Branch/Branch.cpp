#include "Branch.hpp"
#include "../MIPS.hpp"
#include "../Registers.hpp"

namespace nova::ps1::branch {

static inline uint32_t Rs(uint32_t opcode) { return (opcode >> 21) & 0x1F; }
static inline uint32_t Rt(uint32_t opcode) { return (opcode >> 16) & 0x1F; }
static inline uint32_t Rd(uint32_t opcode) { return (opcode >> 11) & 0x1F; }
static inline int16_t Simm16(uint32_t opcode) { return static_cast<int16_t>(opcode & 0xFFFF); }
static inline uint32_t Target(uint32_t opcode) { return (opcode & 0x03FFFFFF) << 2; }

void BEQ(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    if (regs.GetGPR(Rs(opcode)) == regs.GetGPR(Rt(opcode))) {
        uint32_t target = regs.GetPC() + 4 + (static_cast<int32_t>(Simm16(opcode)) << 2);
        cpu.SetBranchTarget(target);
    }
}

void BNE(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    if (regs.GetGPR(Rs(opcode)) != regs.GetGPR(Rt(opcode))) {
        uint32_t target = regs.GetPC() + 4 + (static_cast<int32_t>(Simm16(opcode)) << 2);
        cpu.SetBranchTarget(target);
    }
}

void BLEZ(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    if (static_cast<int32_t>(regs.GetGPR(Rs(opcode))) <= 0) {
        uint32_t target = regs.GetPC() + 4 + (static_cast<int32_t>(Simm16(opcode)) << 2);
        cpu.SetBranchTarget(target);
    }
}

void BGTZ(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    if (static_cast<int32_t>(regs.GetGPR(Rs(opcode))) > 0) {
        uint32_t target = regs.GetPC() + 4 + (static_cast<int32_t>(Simm16(opcode)) << 2);
        cpu.SetBranchTarget(target);
    }
}

void BLTZ(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    if (static_cast<int32_t>(regs.GetGPR(Rs(opcode))) < 0) {
        uint32_t target = regs.GetPC() + 4 + (static_cast<int32_t>(Simm16(opcode)) << 2);
        cpu.SetBranchTarget(target);
    }
}

void BGEZ(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    if (static_cast<int32_t>(regs.GetGPR(Rs(opcode))) >= 0) {
        uint32_t target = regs.GetPC() + 4 + (static_cast<int32_t>(Simm16(opcode)) << 2);
        cpu.SetBranchTarget(target);
    }
}

void BLTZAL(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(31, regs.GetPC() + 8);
    if (static_cast<int32_t>(regs.GetGPR(Rs(opcode))) < 0) {
        uint32_t target = regs.GetPC() + 4 + (static_cast<int32_t>(Simm16(opcode)) << 2);
        cpu.SetBranchTarget(target);
    }
}

void BGEZAL(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(31, regs.GetPC() + 8);
    if (static_cast<int32_t>(regs.GetGPR(Rs(opcode))) >= 0) {
        uint32_t target = regs.GetPC() + 4 + (static_cast<int32_t>(Simm16(opcode)) << 2);
        cpu.SetBranchTarget(target);
    }
}

void J(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t target = (regs.GetPC() & 0xF0000000) | Target(opcode);
    cpu.SetBranchTarget(target);
}

void JAL(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(31, regs.GetPC() + 8);
    uint32_t target = (regs.GetPC() & 0xF0000000) | Target(opcode);
    cpu.SetBranchTarget(target);
}

void JR(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    cpu.SetBranchTarget(regs.GetGPR(Rs(opcode)));
}

void JALR(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rd(opcode), regs.GetPC() + 8);
    cpu.SetBranchTarget(regs.GetGPR(Rs(opcode)));
}

}