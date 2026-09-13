#pragma once
#include "Color.hpp"
#include <cstdint>

namespace nova::ps1 {

enum class BlendMode : uint32_t {
    Half     = 0,
    Add      = 1,
    Subtract = 2,
    AddQuarter = 3
};

class Blending {
public:
    Blending();
    ~Blending() = default;

    void Reset();

    void SetBlendMode(uint32_t mode) { _mode = static_cast<BlendMode>(mode & 0x3); }
    BlendMode GetBlendMode() const { return _mode; }

    Color Blend(const Color& src, const Color& dst) const;

private:
    BlendMode _mode;
};

}