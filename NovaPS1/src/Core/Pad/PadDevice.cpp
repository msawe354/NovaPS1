#include "PadDevice.hpp"

namespace nova::ps1 {

PadDevice::PadDevice() {
    Reset();
}

void PadDevice::Reset() {
    _pad.Reset();
}

uint32_t PadDevice::Read(uint32_t offset, unsigned size) {
    return _pad.ReadRegister(0x1F801040 + offset);
}

void PadDevice::Write(uint32_t offset, uint32_t value, unsigned size) {
    _pad.WriteRegister(0x1F801040 + offset, value);
}

}