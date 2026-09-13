#pragma once
#include <cstdint>

namespace nova::ps1 {
class MIPS;
}

namespace nova::ps1::loadstore {

void LB(MIPS& cpu, uint32_t opcode);
void LBU(MIPS& cpu, uint32_t opcode);
void LH(MIPS& cpu, uint32_t opcode);
void LHU(MIPS& cpu, uint32_t opcode);
void LW(MIPS& cpu, uint32_t opcode);
void LWL(MIPS& cpu, uint32_t opcode);
void LWR(MIPS& cpu, uint32_t opcode);
void SB(MIPS& cpu, uint32_t opcode);
void SH(MIPS& cpu, uint32_t opcode);
void SW(MIPS& cpu, uint32_t opcode);
void SWL(MIPS& cpu, uint32_t opcode);
void SWR(MIPS& cpu, uint32_t opcode);

}