#include "TextureBlend.hpp"

namespace nova::ps1 {

TextureBlend::TextureBlend() : _mode(TextureBlendMode::Modulate) {
}

void TextureBlend::Reset() {
    _mode = TextureBlendMode::Modulate;
}

static inline uint8_t Modulate(uint8_t a, uint8_t b) {
    return static_cast<uint8_t>((a * b) / 255);
}

Color TextureBlend::Blend(const Color& texture, const Color& vertex) const {
    Color result;

    switch (_mode) {
        case TextureBlendMode::Modulate:
            result.r = Modulate(texture.r, vertex.r);
            result.g = Modulate(texture.g, vertex.g);
            result.b = Modulate(texture.b, vertex.b);
            result.a = Modulate(texture.a, vertex.a);
            break;

        case TextureBlendMode::Decal:
            result.r = texture.r;
            result.g = texture.g;
            result.b = texture.b;
            result.a = vertex.a;
            break;

        case TextureBlendMode::Highlight:
            result.r = static_cast<uint8_t>(texture.r + vertex.r > 255 ? 255 : texture.r + vertex.r);
            result.g = static_cast<uint8_t>(texture.g + vertex.g > 255 ? 255 : texture.g + vertex.g);
            result.b = static_cast<uint8_t>(texture.b + vertex.b > 255 ? 255 : texture.b + vertex.b);
            result.a = vertex.a;
            break;

        case TextureBlendMode::Highlight2:
            result.r = Modulate(texture.r, vertex.r * 2 > 255 ? 255 : vertex.r * 2);
            result.g = Modulate(texture.g, vertex.g * 2 > 255 ? 255 : vertex.g * 2);
            result.b = Modulate(texture.b, vertex.b * 2 > 255 ? 255 : vertex.b * 2);
            result.a = vertex.a;
            break;

        default:
            result = texture;
            break;
    }

    return result;
}

}