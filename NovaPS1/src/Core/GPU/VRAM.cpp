#include "VRAM.hpp"

namespace nova::ps1 {

VRAM::VRAM() {
    Reset();
}

void VRAM::Reset() {
    _data.fill(0);
}

uint32_t VRAM::GetOffset(uint32_t x, uint32_t y) const {
    return (y * WIDTH + x) * 2;
}

uint16_t VRAM::Read16(uint32_t x, uint32_t y) const {
    if (x >= WIDTH || y >= HEIGHT) return 0;
    uint32_t offset = GetOffset(x, y);
    return static_cast<uint16_t>(_data[offset] | (_data[offset + 1] << 8));
}

void VRAM::Write16(uint32_t x, uint32_t y, uint16_t value) {
    if (x >= WIDTH || y >= HEIGHT) return;
    uint32_t offset = GetOffset(x, y);
    _data[offset] = static_cast<uint8_t>(value & 0xFF);
    _data[offset + 1] = static_cast<uint8_t>((value >> 8) & 0xFF);
}

}