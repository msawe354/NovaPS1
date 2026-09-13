#include "Logical.hpp"
#include "../MIPS.hpp"
#include "../Registers.hpp"

namespace nova::ps1::logical {

static inline uint32_t Rs(uint32_t opcode) { return (opcode >> 21) & 0x1F; }
static inline uint32_t Rt(uint32_t opcode) { return (opcode >> 16) & 0x1F; }
static inline uint32_t Rd(uint32_t opcode) { return (opcode >> 11) & 0x1F; }
static inline uint32_t Imm16(uint32_t opcode) { return opcode & 0xFFFF; }

void AND(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rd(opcode), regs.GetGPR(Rs(opcode)) & regs.GetGPR(Rt(opcode)));
}

void ANDI(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rt(opcode), regs.GetGPR(Rs(opcode)) & Imm16(opcode));
}

void OR(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rd(opcode), regs.GetGPR(Rs(opcode)) | regs.GetGPR(Rt(opcode)));
}

void ORI(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rt(opcode), regs.GetGPR(Rs(opcode)) | Imm16(opcode));
}

void XOR(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rd(opcode), regs.GetGPR(Rs(opcode)) ^ regs.GetGPR(Rt(opcode)));
}

void XORI(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rt(opcode), regs.GetGPR(Rs(opcode)) ^ Imm16(opcode));
}

void NOR(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rd(opcode), ~(regs.GetGPR(Rs(opcode)) | regs.GetGPR(Rt(opcode))));
}

void LUI(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rt(opcode), Imm16(opcode) << 16);
}

}