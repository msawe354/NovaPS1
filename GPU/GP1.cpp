#include "GP1.hpp"
#include "GPU.hpp"

namespace nova::ps1::gpu {

void ExecuteGP1(GPU& gpu, uint32_t value) {
    uint32_t command = (value >> 24) & 0xFF;

    switch (command) {
        case 0x00:
            gpu.Reset();
            break;
        case 0x01:
            gpu.ResetCommandBuffer();
            break;
        case 0x02:
            gpu.AcknowledgeIRQ();
            break;
        case 0x03:
            gpu.SetDisplayEnable((value & 1) != 0);
            break;
        case 0x04:
            gpu.SetDMADirection(value & 0x3);
            break;
        case 0x05:
            gpu.SetDisplayVRAMStart(value & 0x3FFFFF);
            break;
        case 0x06:
            gpu.SetHorizontalRange(value & 0xFFF, (value >> 12) & 0xFFF);
            break;
        case 0x07:
            gpu.SetVerticalRange(value & 0x3FF, (value >> 10) & 0x3FF);
            break;
        case 0x08:
            gpu.SetDisplayMode(value & 0xFF);
            break;
        case 0x10:
            gpu.SetGPUType(value & 0xFF);
            break;
        default:
            break;
    }
}

}