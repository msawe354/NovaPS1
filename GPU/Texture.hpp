#pragma once
#include <cstdint>

namespace nova::ps1 {

class VRAM;

class Texture {
public:
    Texture();
    ~Texture() = default;

    void Reset();

    void SetVRAM(VRAM* vram) { _vram = vram; }

    void SetTexturePageX(uint32_t value) { _texturePageX = value; }
    void SetTexturePageY(uint32_t value) { _texturePageY = value; }
    void SetTextureDepth(uint32_t depth) { _textureDepth = depth; }
    void SetCLUTX(uint32_t value) { _clutX = value; }
    void SetCLUTY(uint32_t value) { _clutY = value; }

    uint32_t GetTexturePageX() const { return _texturePageX; }
    uint32_t GetTexturePageY() const { return _texturePageY; }
    uint32_t GetTextureDepth() const { return _textureDepth; }
    uint32_t GetCLUTX() const { return _clutX; }
    uint32_t GetCLUTY() const { return _clutY; }

    uint16_t Sample(uint32_t u, uint32_t v) const;

private:
    VRAM* _vram;
    uint32_t _texturePageX;
    uint32_t _texturePageY;
    uint32_t _textureDepth;
    uint32_t _clutX;
    uint32_t _clutY;

    uint16_t Sample4Bit(uint32_t u, uint32_t v) const;
    uint16_t Sample8Bit(uint32_t u, uint32_t v) const;
    uint16_t Sample15Bit(uint32_t u, uint32_t v) const;
    uint16_t ReadCLUT(uint32_t index) const;
};

}