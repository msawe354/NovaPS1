#include "Blending.hpp"

namespace nova::ps1 {

Blending::Blending() : _mode(BlendMode::Half) {
}

void Blending::Reset() {
    _mode = BlendMode::Half;
}

static inline uint8_t Saturate(int32_t value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return static_cast<uint8_t>(value);
}

Color Blending::Blend(const Color& src, const Color& dst) const {
    Color result;

    switch (_mode) {
        case BlendMode::Half:
            result.r = Saturate((src.r + dst.r) / 2);
            result.g = Saturate((src.g + dst.g) / 2);
            result.b = Saturate((src.b + dst.b) / 2);
            break;

        case BlendMode::Add:
            result.r = Saturate(src.r + dst.r);
            result.g = Saturate(src.g + dst.g);
            result.b = Saturate(src.b + dst.b);
            break;

        case BlendMode::Subtract:
            result.r = Saturate(src.r - dst.r);
            result.g = Saturate(src.g - dst.g);
            result.b = Saturate(src.b - dst.b);
            break;

        case BlendMode::AddQuarter:
            result.r = Saturate(src.r + dst.r / 4);
            result.g = Saturate(src.g + dst.g / 4);
            result.b = Saturate(src.b + dst.b / 4);
            break;

        default:
            result = src;
            break;
    }

    result.a = src.a;
    return result;
}

}