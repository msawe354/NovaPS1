#pragma once
#include <cstdint>

namespace nova::ps1 {
class GTE;
}

namespace nova::ps1::gte {

void NCLIP(GTE& gte, uint32_t opcode);
void MVMVA(GTE& gte, uint32_t opcode);
void OP(GTE& gte, uint32_t opcode);
void SQR(GTE& gte, uint32_t opcode);
void GPF(GTE& gte, uint32_t opcode);
void GPL(GTE& gte, uint32_t opcode);

}