#include "Primitive.hpp"

namespace nova::ps1 {

Primitive::Primitive()
    : _type(PrimitiveType::Triangle),
      _shading(ShadingMode::Flat),
      _texture(TextureMode::NoTexture),
      _transparency(SemiTransparency::Opaque),
      _rawTexture(false),
      _dithering(false) {
}

void Primitive::Reset() {
    _type = PrimitiveType::Triangle;
    _shading = ShadingMode::Flat;
    _texture = TextureMode::NoTexture;
    _transparency = SemiTransparency::Opaque;
    _rawTexture = false;
    _dithering = false;
}

void Primitive::ParseCommand(uint32_t firstWord) {
    uint32_t command = (firstWord >> 24) & 0xFF;

    if (command >= 0x20 && command <= 0x3F) {
        if ((command & 0x04) != 0) {
            _type = PrimitiveType::PolyLine;
        } else if ((command & 0x08) != 0) {
            _type = PrimitiveType::Line;
        } else if ((command & 0x10) != 0) {
            _type = PrimitiveType::Triangle;
        } else {
            _type = PrimitiveType::Triangle;
        }

        _shading = (command & 0x01) ? ShadingMode::Gouraud : ShadingMode::Flat;
        _texture = (command & 0x02) ? TextureMode::Textured : TextureMode::NoTexture;
        _transparency = (command & 0x02) ? SemiTransparency::SemiTransparent : SemiTransparency::Opaque;
        _rawTexture = (command & 0x01) != 0;
    } else if (command >= 0x28 && command <= 0x2F) {
        _type = PrimitiveType::Quad;
        _shading = (command & 0x01) ? ShadingMode::Gouraud : ShadingMode::Flat;
        _texture = (command & 0x02) ? TextureMode::Textured : TextureMode::NoTexture;
    } else if (command >= 0x30 && command <= 0x37) {
        _type = PrimitiveType::Quad;
        _shading = (command & 0x01) ? ShadingMode::Gouraud : ShadingMode::Flat;
        _texture = (command & 0x02) ? TextureMode::Textured : TextureMode::NoTexture;
    } else if (command >= 0x38 && command <= 0x3F) {
        _type = PrimitiveType::Quad;
        _shading = (command & 0x01) ? ShadingMode::Gouraud : ShadingMode::Flat;
        _texture = (command & 0x02) ? TextureMode::Textured : TextureMode::NoTexture;
    }
}

uint32_t Primitive::GetVertexCount() const {
    switch (_type) {
        case PrimitiveType::Triangle: return 3;
        case PrimitiveType::Quad: return 4;
        case PrimitiveType::Line: return 2;
        case PrimitiveType::PolyLine: return 0;
        default: return 3;
    }
}

uint32_t Primitive::GetWordCount() const {
    uint32_t vertexCount = GetVertexCount();
    uint32_t wordsPerVertex = 1;

    if (_shading == ShadingMode::Gouraud) {
        wordsPerVertex = _texture == TextureMode::Textured ? 3 : 2;
    } else {
        wordsPerVertex = _texture == TextureMode::Textured ? 2 : 1;
    }

    return 1 + vertexCount * wordsPerVertex;
}

}