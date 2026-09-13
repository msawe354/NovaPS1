#pragma once
#include "Color.hpp"
#include <cstdint>

namespace nova::ps1 {

enum class TextureBlendMode : uint32_t {
    Modulate = 0,
    Decal    = 1,
    Highlight = 2,
    Highlight2 = 3
};

class TextureBlend {
public:
    TextureBlend();
    ~TextureBlend() = default;

    void Reset();

    void SetMode(uint32_t mode) { _mode = static_cast<TextureBlendMode>(mode & 0x3); }
    TextureBlendMode GetMode() const { return _mode; }

    Color Blend(const Color& texture, const Color& vertex) const;

private:
    TextureBlendMode _mode;
};

}