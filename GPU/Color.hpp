#pragma once
#include <cstdint>

namespace nova::ps1 {

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color() : r(0), g(0), b(0), a(0xFF) {}
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF)
        : r(red), g(green), b(blue), a(alpha) {}

    static Color FromRGB24(uint32_t value) {
        return Color(
            static_cast<uint8_t>((value >> 16) & 0xFF),
            static_cast<uint8_t>((value >> 8) & 0xFF),
            static_cast<uint8_t>(value & 0xFF),
            0xFF
        );
    }

    static Color FromRGB15(uint16_t value) {
        uint8_t r = static_cast<uint8_t>((value & 0x1F) << 3);
        uint8_t g = static_cast<uint8_t>(((value >> 5) & 0x1F) << 3);
        uint8_t b = static_cast<uint8_t>(((value >> 10) & 0x1F) << 3);
        return Color(r, g, b, 0xFF);
    }

    uint16_t ToRGB15() const {
        return static_cast<uint16_t>(
            ((r >> 3) & 0x1F) |
            (((g >> 3) & 0x1F) << 5) |
            (((b >> 3) & 0x1F) << 10) |
            (a == 0 ? 0x8000 : 0)
        );
    }

    uint32_t ToRGB24() const {
        return (static_cast<uint32_t>(r) << 16) |
               (static_cast<uint32_t>(g) << 8) |
               static_cast<uint32_t>(b);
    }
};

}