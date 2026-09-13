#pragma once
#include <cstdint>

namespace nova::ps1 {
class MIPS;
}

namespace nova::ps1::fpu {

void ADD_S(MIPS& cpu, uint32_t opcode);
void ADD_D(MIPS& cpu, uint32_t opcode);
void SUB_S(MIPS& cpu, uint32_t opcode);
void SUB_D(MIPS& cpu, uint32_t opcode);
void MUL_S(MIPS& cpu, uint32_t opcode);
void MUL_D(MIPS& cpu, uint32_t opcode);
void DIV_S(MIPS& cpu, uint32_t opcode);
void DIV_D(MIPS& cpu, uint32_t opcode);
void SQRT_S(MIPS& cpu, uint32_t opcode);
void ABS_S(MIPS& cpu, uint32_t opcode);
void MOV_S(MIPS& cpu, uint32_t opcode);
void NEG_S(MIPS& cpu, uint32_t opcode);
void CVT_S_W(MIPS& cpu, uint32_t opcode);
void CVT_W_S(MIPS& cpu, uint32_t opcode);
void CVT_S_D(MIPS& cpu, uint32_t opcode);
void CVT_D_S(MIPS& cpu, uint32_t opcode);
void CVT_W_D(MIPS& cpu, uint32_t opcode);
void CVT_D_W(MIPS& cpu, uint32_t opcode);
void C_EQ_S(MIPS& cpu, uint32_t opcode);
void C_LT_S(MIPS& cpu, uint32_t opcode);
void C_LE_S(MIPS& cpu, uint32_t opcode);
void C_EQ_D(MIPS& cpu, uint32_t opcode);
void C_LT_D(MIPS& cpu, uint32_t opcode);
void C_LE_D(MIPS& cpu, uint32_t opcode);
void MFC1(MIPS& cpu, uint32_t opcode);
void MTC1(MIPS& cpu, uint32_t opcode);
void CFC1(MIPS& cpu, uint32_t opcode);
void CTC1(MIPS& cpu, uint32_t opcode);
void LWC1(MIPS& cpu, uint32_t opcode);
void SWC1(MIPS& cpu, uint32_t opcode);

}