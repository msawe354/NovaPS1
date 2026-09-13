#pragma once
#include <cstdint>

namespace nova::ps1 {
class GTE;
}

namespace nova::ps1::gte {

void DPCL(GTE& gte, uint32_t opcode);
void DPST(GTE& gte, uint32_t opcode);
void AVSZ3(GTE& gte, uint32_t opcode);
void AVSZ4(GTE& gte, uint32_t opcode);

}