#pragma once
#include <cstdint>

namespace nova::ps1 {
class MIPS;
}

namespace nova::ps1::arithmetic {

void ADD(MIPS& cpu, uint32_t opcode);
void ADDU(MIPS& cpu, uint32_t opcode);
void ADDI(MIPS& cpu, uint32_t opcode);
void ADDIU(MIPS& cpu, uint32_t opcode);
void SUB(MIPS& cpu, uint32_t opcode);
void SUBU(MIPS& cpu, uint32_t opcode);
void MULT(MIPS& cpu, uint32_t opcode);
void MULTU(MIPS& cpu, uint32_t opcode);
void DIV(MIPS& cpu, uint32_t opcode);
void DIVU(MIPS& cpu, uint32_t opcode);
void MFHI(MIPS& cpu, uint32_t opcode);
void MFLO(MIPS& cpu, uint32_t opcode);
void MTHI(MIPS& cpu, uint32_t opcode);
void MTLO(MIPS& cpu, uint32_t opcode);
void SLT(MIPS& cpu, uint32_t opcode);
void SLTU(MIPS& cpu, uint32_t opcode);
void SLTI(MIPS& cpu, uint32_t opcode);
void SLTIU(MIPS& cpu, uint32_t opcode);

}