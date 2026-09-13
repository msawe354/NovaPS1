#include "Arithmetic.hpp"
#include "../MIPS.hpp"
#include "../Registers.hpp"

namespace nova::ps1::arithmetic {

static inline uint32_t Rs(uint32_t opcode) { return (opcode >> 21) & 0x1F; }
static inline uint32_t Rt(uint32_t opcode) { return (opcode >> 16) & 0x1F; }
static inline uint32_t Rd(uint32_t opcode) { return (opcode >> 11) & 0x1F; }
static inline int16_t Simm16(uint32_t opcode) { return static_cast<int16_t>(opcode & 0xFFFF); }

void ADD(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    int32_t a = static_cast<int32_t>(regs.GetGPR(Rs(opcode)));
    int32_t b = static_cast<int32_t>(regs.GetGPR(Rt(opcode)));
    int32_t result = a + b;

    if (((a ^ result) & (b ^ result)) < 0) {
        cpu.HandleException(12);
        return;
    }

    regs.SetGPR(Rd(opcode), static_cast<uint32_t>(result));
}

void ADDU(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rd(opcode), regs.GetGPR(Rs(opcode)) + regs.GetGPR(Rt(opcode)));
}

void ADDI(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    int32_t a = static_cast<int32_t>(regs.GetGPR(Rs(opcode)));
    int32_t imm = Simm16(opcode);
    int32_t result = a + imm;

    if (((a ^ result) & (imm ^ result)) < 0) {
        cpu.HandleException(12);
        return;
    }

    regs.SetGPR(Rt(opcode), static_cast<uint32_t>(result));
}

void ADDIU(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rt(opcode), regs.GetGPR(Rs(opcode)) + static_cast<uint32_t>(Simm16(opcode)));
}

void SUB(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    int32_t a = static_cast<int32_t>(regs.GetGPR(Rs(opcode)));
    int32_t b = static_cast<int32_t>(regs.GetGPR(Rt(opcode)));
    int32_t result = a - b;

    if (((a ^ b) & (a ^ result)) < 0) {
        cpu.HandleException(12);
        return;
    }

    regs.SetGPR(Rd(opcode), static_cast<uint32_t>(result));
}

void SUBU(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rd(opcode), regs.GetGPR(Rs(opcode)) - regs.GetGPR(Rt(opcode)));
}

void MULT(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    int64_t a = static_cast<int64_t>(static_cast<int32_t>(regs.GetGPR(Rs(opcode))));
    int64_t b = static_cast<int64_t>(static_cast<int32_t>(regs.GetGPR(Rt(opcode))));
    int64_t result = a * b;

    regs.SetLO(static_cast<uint32_t>(result & 0xFFFFFFFF));
    regs.SetHI(static_cast<uint32_t>((result >> 32) & 0xFFFFFFFF));
}

void MULTU(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint64_t a = static_cast<uint64_t>(regs.GetGPR(Rs(opcode)));
    uint64_t b = static_cast<uint64_t>(regs.GetGPR(Rt(opcode)));
    uint64_t result = a * b;

    regs.SetLO(static_cast<uint32_t>(result & 0xFFFFFFFF));
    regs.SetHI(static_cast<uint32_t>((result >> 32) & 0xFFFFFFFF));
}

void DIV(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    int32_t a = static_cast<int32_t>(regs.GetGPR(Rs(opcode)));
    int32_t b = static_cast<int32_t>(regs.GetGPR(Rt(opcode)));

    if (b == 0) {
        regs.SetLO(a >= 0 ? 0xFFFFFFFF : 0x00000001);
        regs.SetHI(static_cast<uint32_t>(a));
        return;
    }

    if (a == INT32_MIN && b == -1) {
        regs.SetLO(static_cast<uint32_t>(INT32_MIN));
        regs.SetHI(0);
        return;
    }

    regs.SetLO(static_cast<uint32_t>(a / b));
    regs.SetHI(static_cast<uint32_t>(a % b));
}

void DIVU(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t a = regs.GetGPR(Rs(opcode));
    uint32_t b = regs.GetGPR(Rt(opcode));

    if (b == 0) {
        regs.SetLO(0xFFFFFFFF);
        regs.SetHI(a);
        return;
    }

    regs.SetLO(a / b);
    regs.SetHI(a % b);
}

void MFHI(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rd(opcode), regs.GetHI());
}

void MFLO(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rd(opcode), regs.GetLO());
}

void MTHI(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetHI(regs.GetGPR(Rs(opcode)));
}

void MTLO(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetLO(regs.GetGPR(Rs(opcode)));
}

void SLT(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    int32_t a = static_cast<int32_t>(regs.GetGPR(Rs(opcode)));
    int32_t b = static_cast<int32_t>(regs.GetGPR(Rt(opcode)));
    regs.SetGPR(Rd(opcode), a < b ? 1 : 0);
}

void SLTU(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rd(opcode), regs.GetGPR(Rs(opcode)) < regs.GetGPR(Rt(opcode)) ? 1 : 0);
}

void SLTI(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    int32_t a = static_cast<int32_t>(regs.GetGPR(Rs(opcode)));
    regs.SetGPR(Rt(opcode), a < Simm16(opcode) ? 1 : 0);
}

void SLTIU(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t a = regs.GetGPR(Rs(opcode));
    uint32_t imm = static_cast<uint32_t>(Simm16(opcode));
    regs.SetGPR(Rt(opcode), a < imm ? 1 : 0);
}

}