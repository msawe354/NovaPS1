#include "Texture.hpp"
#include "VRAM.hpp"

namespace nova::ps1 {

Texture::Texture()
    : _vram(nullptr),
      _texturePageX(0),
      _texturePageY(0),
      _textureDepth(0),
      _clutX(0),
      _clutY(0) {
}

void Texture::Reset() {
    _texturePageX = 0;
    _texturePageY = 0;
    _textureDepth = 0;
    _clutX = 0;
    _clutY = 0;
}

uint16_t Texture::ReadCLUT(uint32_t index) const {
    if (!_vram) return 0;
    uint32_t x = _clutX + index;
    uint32_t y = _clutY;
    return _vram->Read16(x, y);
}

uint16_t Texture::Sample4Bit(uint32_t u, uint32_t v) const {
    if (!_vram) return 0;

    uint32_t texX = _texturePageX + u;
    uint32_t texY = _texturePageY + v;

    uint32_t vramX = texX / 4;
    uint32_t vramY = texY;

    uint16_t data = _vram->Read16(vramX, vramY);
    uint32_t shift = (3 - (u & 0x3)) * 4;
    uint32_t index = (data >> shift) & 0xF;

    return ReadCLUT(index);
}

uint16_t Texture::Sample8Bit(uint32_t u, uint32_t v) const {
    if (!_vram) return 0;

    uint32_t texX = _texturePageX + u;
    uint32_t texY = _texturePageY + v;

    uint32_t vramX = texX / 2;
    uint32_t vramY = texY;

    uint16_t data = _vram->Read16(vramX, vramY);
    uint32_t shift = (1 - (u & 0x1)) * 8;
    uint32_t index = (data >> shift) & 0xFF;

    return ReadCLUT(index);
}

uint16_t Texture::Sample15Bit(uint32_t u, uint32_t v) const {
    if (!_vram) return 0;

    uint32_t vramX = _texturePageX + u;
    uint32_t vramY = _texturePageY + v;

    return _vram->Read16(vramX, vramY);
}

uint16_t Texture::Sample(uint32_t u, uint32_t v) const {
    switch (_textureDepth) {
        case 0: return Sample4Bit(u, v);
        case 1: return Sample8Bit(u, v);
        case 2: return Sample15Bit(u, v);
        default: return 0;
    }
}

}