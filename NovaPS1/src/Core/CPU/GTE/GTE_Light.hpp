#pragma once
#include <cstdint>

namespace nova::ps1 {
class GTE;
}

namespace nova::ps1::gte {

void NCS(GTE& gte, uint32_t opcode);
void NCT(GTE& gte, uint32_t opcode);
void NCDS(GTE& gte, uint32_t opcode);
void NCDT(GTE& gte, uint32_t opcode);
void NCCS(GTE& gte, uint32_t opcode);
void NCCT(GTE& gte, uint32_t opcode);
void CC(GTE& gte, uint32_t opcode);
void CDP(GTE& gte, uint32_t opcode);

}