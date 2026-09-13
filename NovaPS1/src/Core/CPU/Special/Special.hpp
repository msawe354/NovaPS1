#pragma once
#include <cstdint>

namespace nova::ps1 {
class MIPS;
}

namespace nova::ps1::special {

void SYSCALL(MIPS& cpu, uint32_t opcode);
void BREAK(MIPS& cpu, uint32_t opcode);
void NOP(MIPS& cpu, uint32_t opcode);

}