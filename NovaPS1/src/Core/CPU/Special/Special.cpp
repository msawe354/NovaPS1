#include "Special.hpp"
#include "../MIPS.hpp"
#include "../Registers.hpp"
#include <iostream>

namespace nova::ps1::special {

void SYSCALL(MIPS& cpu, uint32_t opcode) {
    cpu.HandleException(8);
}

void BREAK(MIPS& cpu, uint32_t opcode) {
    cpu.HandleException(9);
}

void NOP(MIPS& cpu, uint32_t opcode) {
}

}