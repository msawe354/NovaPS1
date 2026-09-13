#pragma once
#include <cstdint>

namespace nova::ps1 {
class GTE;
}

namespace nova::ps1::gte {

void RTPS(GTE& gte, uint32_t opcode);
void RTPT(GTE& gte, uint32_t opcode);

}