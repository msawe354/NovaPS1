#pragma once
#include "Vertex.hpp"
#include "Color.hpp"
#include "Texture.hpp"
#include "Blending.hpp"
#include "TextureBlend.hpp"
#include <cstdint>
#include <vector>

namespace nova::ps1 {

class VRAM;
class TextureCache;

class Renderer {
public:
    Renderer();
    ~Renderer() = default;

    void Reset();

    void SetVRAM(VRAM* vram) { _vram = vram; }
    void SetTextureCache(TextureCache* cache) { _textureCache = cache; }
    void SetTexture(Texture* texture) { _texture = texture; }
    void SetBlending(Blending* blending) { _blending = blending; }
    void SetTextureBlend(TextureBlend* blend) { _textureBlend = blend; }

    void SetTextureEnabled(bool enabled) { _textureEnabled = enabled; }
    void SetSemiTransparent(bool enabled) { _semiTransparent = enabled; }
    void SetGouraudShading(bool enabled) { _gouraud = enabled; }

    void DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);
    void DrawQuad(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3);
    void DrawLine(const Vertex& v0, const Vertex& v1);

private:
    VRAM* _vram;
    TextureCache* _textureCache;
    Texture* _texture;
    Blending* _blending;
    TextureBlend* _textureBlend;

    bool _textureEnabled;
    bool _semiTransparent;
    bool _gouraud;

    void PlotPixel(int16_t x, int16_t y, const Color& color);
    Color FetchVertexColor(const Vertex& v) const;
    Color FetchTextureColor(const Vertex& v) const;
    Color BlendColors(const Color& src, const Color& dst) const;
    void RasterizeFlat(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Color& color);
    void RasterizeGouraud(const Vertex& v0, const Vertex& v1, const Vertex& v2);
    bool IsInsideTriangle(int16_t px, int16_t py,
                          int16_t x0, int16_t y0,
                          int16_t x1, int16_t y1,
                          int16_t x2, int16_t y2) const;
};

}