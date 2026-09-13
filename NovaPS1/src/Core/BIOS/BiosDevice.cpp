#include "BiosDevice.hpp"

namespace nova::ps1 {

BiosDevice::BiosDevice() {
    Reset();
}

void BiosDevice::Reset() {
    _bios.Reset();
}

bool BiosDevice::Load(const std::string& path) {
    return _bios.Load(path);
}

uint32_t BiosDevice::Read(uint32_t offset, unsigned size) {
    switch (size) {
        case 1: return _bios.Read8(offset);
        case 2: return _bios.Read16(offset);
        case 4: return _bios.Read32(offset);
        default: return 0;
    }
}

void BiosDevice::Write(uint32_t offset, uint32_t value, unsigned size) {
    (void)offset;
    (void)value;
    (void)size;
}

}