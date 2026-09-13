#include "GP0.hpp"
#include "GPU.hpp"

namespace nova::ps1::gpu {

void ExecuteGP0(GPU& gpu, uint32_t value) {
    uint32_t command = (value >> 24) & 0xFF;
    (void)command;
    (void)gpu;
}

}