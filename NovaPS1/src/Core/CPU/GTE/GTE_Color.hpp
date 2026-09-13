#pragma once
#include <cstdint>

namespace nova::ps1 {
class GTE;
}

namespace nova::ps1::gte {

void DPCS(GTE& gte, uint32_t opcode);
void DPCT(GTE& gte, uint32_t opcode);
void DCPL(GTE& gte, uint32_t opcode);
void INTPL(GTE& gte, uint32_t opcode);

}