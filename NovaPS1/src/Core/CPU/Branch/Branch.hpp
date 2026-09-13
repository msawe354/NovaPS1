#pragma once
#include <cstdint>

namespace nova::ps1 {
class MIPS;
}

namespace nova::ps1::branch {

void BEQ(MIPS& cpu, uint32_t opcode);
void BNE(MIPS& cpu, uint32_t opcode);
void BLEZ(MIPS& cpu, uint32_t opcode);
void BGTZ(MIPS& cpu, uint32_t opcode);
void BLTZ(MIPS& cpu, uint32_t opcode);
void BGEZ(MIPS& cpu, uint32_t opcode);
void BLTZAL(MIPS& cpu, uint32_t opcode);
void BGEZAL(MIPS& cpu, uint32_t opcode);
void J(MIPS& cpu, uint32_t opcode);
void JAL(MIPS& cpu, uint32_t opcode);
void JR(MIPS& cpu, uint32_t opcode);
void JALR(MIPS& cpu, uint32_t opcode);

}