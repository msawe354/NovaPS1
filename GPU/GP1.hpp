#pragma once
#include <cstdint>

namespace nova::ps1 {
class GPU;
}

namespace nova::ps1::gpu {

void ExecuteGP1(GPU& gpu, uint32_t value);

}