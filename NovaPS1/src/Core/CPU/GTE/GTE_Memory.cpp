#include "GTE_Memory.hpp"
#include "GTE.hpp"
#include "../MIPS.hpp"
#include "../Registers.hpp"
#include "../../Memory/Bus.hpp"

namespace nova::ps1::gte {

static inline uint32_t Rs(uint32_t opcode) { return (opcode >> 21) & 0x1F; }
static inline uint32_t Rt(uint32_t opcode) { return (opcode >> 16) & 0x1F; }
static inline uint32_t Rd(uint32_t opcode) { return (opcode >> 11) & 0x1F; }
static inline int16_t Simm16(uint32_t opcode) { return static_cast<int16_t>(opcode & 0xFFFF); }

void MFC2(MIPS& cpu, uint32_t opcode) {
    (void)cpu;
    (void)opcode;
}

void MTC2(MIPS& cpu, uint32_t opcode) {
    (void)cpu;
    (void)opcode;
}

void CFC2(MIPS& cpu, uint32_t opcode) {
    (void)cpu;
    (void)opcode;
}

void CTC2(MIPS& cpu, uint32_t opcode) {
    (void)cpu;
    (void)opcode;
}

void LWC2(MIPS& cpu, uint32_t opcode) {
    (void)cpu;
    (void)opcode;
}

void SWC2(MIPS& cpu, uint32_t opcode) {
    (void)cpu;
    (void)opcode;
}

}