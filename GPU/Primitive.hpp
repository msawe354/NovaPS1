#pragma once
#include "Vertex.hpp"
#include <cstdint>
#include <vector>

namespace nova::ps1 {

enum class PrimitiveType : uint32_t {
    Triangle = 0,
    Quad = 1,
    Line = 2,
    PolyLine = 3
};

enum class ShadingMode : uint32_t {
    Flat = 0,
    Gouraud = 1
};

enum class TextureMode : uint32_t {
    NoTexture = 0,
    Textured = 1
};

enum class SemiTransparency : uint32_t {
    Opaque = 0,
    SemiTransparent = 1
};

enum class RawTextureMode : uint32_t {
    TextureBlend = 0,
    RawTexture = 1
};

struct PrimitiveInfo {
    PrimitiveType type;
    ShadingMode shading;
    TextureMode texture;
    SemiTransparency transparency;
    RawTextureMode rawTexture;
    bool textured;
    bool gouraud;
    bool semiTransparent;
    bool rawTextureMode;
    bool dithering;
};

class Primitive {
public:
    Primitive();
    ~Primitive() = default;

    void Reset();

    void ParseCommand(uint32_t firstWord);

    PrimitiveType GetType() const { return _type; }
    ShadingMode GetShading() const { return _shading; }
    TextureMode GetTexture() const { return _texture; }
    SemiTransparency GetTransparency() const { return _transparency; }
    bool IsRawTexture() const { return _rawTexture; }

    uint32_t GetVertexCount() const;
    uint32_t GetWordCount() const;

private:
    PrimitiveType _type;
    ShadingMode _shading;
    TextureMode _texture;
    SemiTransparency _transparency;
    bool _rawTexture;
    bool _dithering;
};

}