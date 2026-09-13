#include "LoadStore.hpp"
#include "../MIPS.hpp"
#include "../Registers.hpp"
#include "../../Memory/Bus.hpp"

namespace nova::ps1::loadstore {

static inline uint32_t Rs(uint32_t opcode) { return (opcode >> 21) & 0x1F; }
static inline uint32_t Rt(uint32_t opcode) { return (opcode >> 16) & 0x1F; }
static inline int16_t Simm16(uint32_t opcode) { return static_cast<int16_t>(opcode & 0xFFFF); }

void LB(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t addr = regs.GetGPR(Rs(opcode)) + static_cast<uint32_t>(Simm16(opcode));
    int8_t value = static_cast<int8_t>(cpu.GetBus().Read8(addr));
    regs.SetGPR(Rt(opcode), static_cast<uint32_t>(static_cast<int32_t>(value)));
}

void LBU(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t addr = regs.GetGPR(Rs(opcode)) + static_cast<uint32_t>(Simm16(opcode));
    regs.SetGPR(Rt(opcode), cpu.GetBus().Read8(addr));
}

void LH(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t addr = regs.GetGPR(Rs(opcode)) + static_cast<uint32_t>(Simm16(opcode));
    int16_t value = static_cast<int16_t>(cpu.GetBus().Read16(addr));
    regs.SetGPR(Rt(opcode), static_cast<uint32_t>(static_cast<int32_t>(value)));
}

void LHU(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t addr = regs.GetGPR(Rs(opcode)) + static_cast<uint32_t>(Simm16(opcode));
    regs.SetGPR(Rt(opcode), cpu.GetBus().Read16(addr));
}

void LW(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t addr = regs.GetGPR(Rs(opcode)) + static_cast<uint32_t>(Simm16(opcode));
    regs.SetGPR(Rt(opcode), cpu.GetBus().Read32(addr));
}

void LWL(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t addr = regs.GetGPR(Rs(opcode)) + static_cast<uint32_t>(Simm16(opcode));
    uint32_t aligned = addr & ~0x3u;
    uint32_t value = cpu.GetBus().Read32(aligned);
    uint32_t shift = (addr & 0x3) * 8;
    uint32_t mask = 0xFFFFFFFFu << (24 - shift);
    uint32_t reg = regs.GetGPR(Rt(opcode));
    regs.SetGPR(Rt(opcode), (reg & ~mask) | (value << (24 - shift)));
}

void LWR(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t addr = regs.GetGPR(Rs(opcode)) + static_cast<uint32_t>(Simm16(opcode));
    uint32_t aligned = addr & ~0x3u;
    uint32_t value = cpu.GetBus().Read32(aligned);
    uint32_t shift = (addr & 0x3) * 8;
    uint32_t mask = 0xFFFFFFFFu >> shift;
    uint32_t reg = regs.GetGPR(Rt(opcode));
    regs.SetGPR(Rt(opcode), (reg & ~mask) | (value >> shift));
}

void SB(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t addr = regs.GetGPR(Rs(opcode)) + static_cast<uint32_t>(Simm16(opcode));
    cpu.GetBus().Write8(addr, static_cast<uint8_t>(regs.GetGPR(Rt(opcode))));
}

void SH(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t addr = regs.GetGPR(Rs(opcode)) + static_cast<uint32_t>(Simm16(opcode));
    cpu.GetBus().Write16(addr, static_cast<uint16_t>(regs.GetGPR(Rt(opcode))));
}

void SW(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t addr = regs.GetGPR(Rs(opcode)) + static_cast<uint32_t>(Simm16(opcode));
    cpu.GetBus().Write32(addr, regs.GetGPR(Rt(opcode)));
}

void SWL(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t addr = regs.GetGPR(Rs(opcode)) + static_cast<uint32_t>(Simm16(opcode));
    uint32_t aligned = addr & ~0x3u;
    uint32_t value = regs.GetGPR(Rt(opcode));
    uint32_t mem = cpu.GetBus().Read32(aligned);
    uint32_t shift = (addr & 0x3) * 8;
    uint32_t mask = 0xFFFFFFFFu << (24 - shift);
    cpu.GetBus().Write32(aligned, (mem & ~mask) | (value << (24 - shift)));
}

void SWR(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t addr = regs.GetGPR(Rs(opcode)) + static_cast<uint32_t>(Simm16(opcode));
    uint32_t aligned = addr & ~0x3u;
    uint32_t value = regs.GetGPR(Rt(opcode));
    uint32_t mem = cpu.GetBus().Read32(aligned);
    uint32_t shift = (addr & 0x3) * 8;
    uint32_t mask = 0xFFFFFFFFu >> shift;
    cpu.GetBus().Write32(aligned, (mem & ~mask) | (value >> shift));
}

}