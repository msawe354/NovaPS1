#pragma once
#include <cstdint>

namespace nova::ps1 {
class GTE;
class MIPS;
}

namespace nova::ps1::gte {

void MFC2(MIPS& cpu, uint32_t opcode);
void MTC2(MIPS& cpu, uint32_t opcode);
void CFC2(MIPS& cpu, uint32_t opcode);
void CTC2(MIPS& cpu, uint32_t opcode);
void LWC2(MIPS& cpu, uint32_t opcode);
void SWC2(MIPS& cpu, uint32_t opcode);

}