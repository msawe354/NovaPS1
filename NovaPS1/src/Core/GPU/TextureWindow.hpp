#pragma once
#include <cstdint>

namespace nova::ps1 {

class TextureWindow {
public:
    TextureWindow();
    ~TextureWindow() = default;

    void Reset();

    void Set(uint32_t value);

    uint32_t ApplyX(uint32_t x) const;
    uint32_t ApplyY(uint32_t y) const;

private:
    uint32_t _maskX;
    uint32_t _maskY;
    uint32_t _offsetX;
    uint32_t _offsetY;
};

}