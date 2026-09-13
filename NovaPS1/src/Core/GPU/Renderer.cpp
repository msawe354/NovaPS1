#include "Renderer.hpp"
#include "VRAM.hpp"
#include "TextureCache.hpp"
#include <algorithm>

namespace nova::ps1 {

Renderer::Renderer()
    : _vram(nullptr),
      _textureCache(nullptr),
      _texture(nullptr),
      _blending(nullptr),
      _textureBlend(nullptr),
      _textureEnabled(false),
      _semiTransparent(false),
      _gouraud(false) {
}

void Renderer::Reset() {
    _textureEnabled = false;
    _semiTransparent = false;
    _gouraud = false;
}

void Renderer::PlotPixel(int16_t x, int16_t y, const Color& color) {
    if (!_vram) return;
    if (x < 0 || y < 0) return;
    if (x >= static_cast<int16_t>(VRAM::WIDTH)) return;
    if (y >= static_cast<int16_t>(VRAM::HEIGHT)) return;

    uint16_t existing = _vram->Read16(x, y);

    Color dst = Color::FromRGB15(existing);
    Color blended = color;

    if (_semiTransparent && _blending) {
        blended = _blending->Blend(color, dst);
    }

    uint16_t output = blended.ToRGB15();
    _vram->Write16(x, y, output);
}

Color Renderer::FetchVertexColor(const Vertex& v) const {
    return Color(v.r, v.g, v.b, 0xFF);
}

Color Renderer::FetchTextureColor(const Vertex& v) const {
    if (!_textureCache) {
        return Color(0xFF, 0xFF, 0xFF, 0xFF);
    }

    uint16_t texel = _textureCache->Sample(v.u, v.v);
    return Color::FromRGB15(texel);
}

Color Renderer::BlendColors(const Color& src, const Color& dst) const {
    if (!_textureBlend) return src;
    return _textureBlend->Blend(src, dst);
}

bool Renderer::IsInsideTriangle(int16_t px, int16_t py,
                                 int16_t x0, int16_t y0,
                                 int16_t x1, int16_t y1,
                                 int16_t x2, int16_t y2) const {
    int32_t w0 = (x1 - x0) * (py - y0) - (y1 - y0) * (px - x0);
    int32_t w1 = (x2 - x1) * (py - y1) - (y2 - y1) * (px - x1);
    int32_t w2 = (x0 - x2) * (py - y2) - (y0 - y2) * (px - x2);

    bool hasNeg = (w0 < 0) || (w1 < 0) || (w2 < 0);
    bool hasPos = (w0 > 0) || (w1 > 0) || (w2 > 0);

    return !(hasNeg && hasPos);
}

void Renderer::RasterizeFlat(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Color& color) {
    int16_t minX = std::min({v0.x, v1.x, v2.x});
    int16_t maxX = std::max({v0.x, v1.x, v2.x});
    int16_t minY = std::min({v0.y, v1.y, v2.y});
    int16_t maxY = std::max({v0.y, v1.y, v2.y});

    minX = std::max<int16_t>(minX, 0);
    minY = std::max<int16_t>(minY, 0);
    maxX = std::min<int16_t>(maxX, static_cast<int16_t>(VRAM::WIDTH - 1));
    maxY = std::min<int16_t>(maxY, static_cast<int16_t>(VRAM::HEIGHT - 1));

    for (int16_t py = minY; py <= maxY; py++) {
        for (int16_t px = minX; px <= maxX; px++) {
            if (IsInsideTriangle(px, py, v0.x, v0.y, v1.x, v1.y, v2.x, v2.y)) {
                Color finalColor = color;

                if (_textureEnabled) {
                    Color texColor = FetchTextureColor(v0);
                    finalColor = BlendColors(texColor, color);
                }

                PlotPixel(px, py, finalColor);
            }
        }
    }
}

void Renderer::RasterizeGouraud(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
    int16_t minX = std::min({v0.x, v1.x, v2.x});
    int16_t maxX = std::max({v0.x, v1.x, v2.x});
    int16_t minY = std::min({v0.y, v1.y, v2.y});
    int16_t maxY = std::max({v0.y, v1.y, v2.y});

    minX = std::max<int16_t>(minX, 0);
    minY = std::max<int16_t>(minY, 0);
    maxX = std::min<int16_t>(maxX, static_cast<int16_t>(VRAM::WIDTH - 1));
    maxY = std::min<int16_t>(maxY, static_cast<int16_t>(VRAM::HEIGHT - 1));

    int32_t area = (v1.x - v0.x) * (v2.y - v0.y) - (v2.x - v0.x) * (v1.y - v0.y);
    if (area == 0) return;

    for (int16_t py = minY; py <= maxY; py++) {
        for (int16_t px = minX; px <= maxX; px++) {
            if (!IsInsideTriangle(px, py, v0.x, v0.y, v1.x, v1.y, v2.x, v2.y)) continue;

            int32_t w0 = (v1.x - px) * (v2.y - py) - (v2.x - px) * (v1.y - py);
            int32_t w1 = (v2.x - px) * (v0.y - py) - (v0.x - px) * (v2.y - py);
            int32_t w2 = area - w0 - w1;

            int32_t r = (w0 * v0.r + w1 * v1.r + w2 * v2.r) / area;
            int32_t g = (w0 * v0.g + w1 * v1.g + w2 * v2.g) / area;
            int32_t b = (w0 * v0.b + w1 * v1.b + w2 * v2.b) / area;

            r = std::clamp(r, 0, 255);
            g = std::clamp(g, 0, 255);
            b = std::clamp(b, 0, 255);

            Color finalColor(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), 0xFF);

            if (_textureEnabled) {
                Color texColor = FetchTextureColor(v0);
                finalColor = BlendColors(texColor, finalColor);
            }

            PlotPixel(px, py, finalColor);
        }
    }
}

void Renderer::DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
    if (_gouraud) {
        RasterizeGouraud(v0, v1, v2);
    } else {
        Color flatColor = FetchVertexColor(v0);
        RasterizeFlat(v0, v1, v2, flatColor);
    }
}

void Renderer::DrawQuad(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3) {
    DrawTriangle(v0, v1, v2);
    DrawTriangle(v0, v2, v3);
}

void Renderer::DrawLine(const Vertex& v0, const Vertex& v1) {
    int16_t dx = static_cast<int16_t>(std::abs(v1.x - v0.x));
    int16_t dy = static_cast<int16_t>(std::abs(v1.y - v0.y));
    int16_t sx = v0.x < v1.x ? 1 : -1;
    int16_t sy = v0.y < v1.y ? 1 : -1;
    int16_t err = dx - dy;

    Color color = FetchVertexColor(v0);

    int16_t x = v0.x;
    int16_t y = v0.y;

    while (true) {
        PlotPixel(x, y, color);
        if (x == v1.x && y == v1.y) break;
        int16_t e2 = static_cast<int16_t>(2 * err);
        if (e2 > -dy) { err -= dy; x += sx; }
        if (e2 < dx) { err += dx; y += sy; }
    }
}

}