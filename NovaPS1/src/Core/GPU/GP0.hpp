#pragma once
#include <cstdint>

namespace nova::ps1 {
class GPU;
}

namespace nova::ps1::gpu {

void ExecuteGP0(GPU& gpu, uint32_t value);

}