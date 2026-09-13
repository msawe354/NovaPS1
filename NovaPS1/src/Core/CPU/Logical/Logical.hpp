#pragma once
#include <cstdint>

namespace nova::ps1 {
class MIPS;
}

namespace nova::ps1::logical {

void AND(MIPS& cpu, uint32_t opcode);
void ANDI(MIPS& cpu, uint32_t opcode);
void OR(MIPS& cpu, uint32_t opcode);
void ORI(MIPS& cpu, uint32_t opcode);
void XOR(MIPS& cpu, uint32_t opcode);
void XORI(MIPS& cpu, uint32_t opcode);
void NOR(MIPS& cpu, uint32_t opcode);
void LUI(MIPS& cpu, uint32_t opcode);

}