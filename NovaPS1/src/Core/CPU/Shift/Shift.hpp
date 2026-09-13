#pragma once
#include <cstdint>

namespace nova::ps1 {
class MIPS;
}

namespace nova::ps1::shift {

void SLL(MIPS& cpu, uint32_t opcode);
void SRL(MIPS& cpu, uint32_t opcode);
void SRA(MIPS& cpu, uint32_t opcode);
void SLLV(MIPS& cpu, uint32_t opcode);
void SRLV(MIPS& cpu, uint32_t opcode);
void SRAV(MIPS& cpu, uint32_t opcode);
void ROL(MIPS& cpu, uint32_t opcode);
void ROR(MIPS& cpu, uint32_t opcode);

}