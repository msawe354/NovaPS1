#include "Rasterizer.hpp"
#include "VRAM.hpp"
#include <algorithm>

namespace nova::ps1 {

Rasterizer::Rasterizer() : _vram(nullptr) {
}

void Rasterizer::Reset() {
}

void Rasterizer::PlotPixel(int16_t x, int16_t y, uint16_t color) {
    if (!_vram) return;
    if (x < 0 || y < 0) return;
    if (x >= static_cast<int16_t>(VRAM::WIDTH)) return;
    if (y >= static_cast<int16_t>(VRAM::HEIGHT)) return;
    _vram->Write16(x, y, color);
}

void Rasterizer::DrawRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color) {
    for (uint16_t j = 0; j < height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            PlotPixel(x + i, y + j, color);
        }
    }
}

void Rasterizer::FillRect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color) {
    DrawRectangle(x, y, width, height, color);
}

bool Rasterizer::IsInsideTriangle(int16_t px, int16_t py,
                                   int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                   int16_t x2, int16_t y2) {
    int32_t w0 = (x1 - x0) * (py - y0) - (y1 - y0) * (px - x0);
    int32_t w1 = (x2 - x1) * (py - y1) - (y2 - y1) * (px - x1);
    int32_t w2 = (x0 - x2) * (py - y2) - (y0 - y2) * (px - x2);

    bool hasNeg = (w0 < 0) || (w1 < 0) || (w2 < 0);
    bool hasPos = (w0 > 0) || (w1 > 0) || (w2 > 0);

    return !(hasNeg && hasPos);
}

void Rasterizer::DrawFlatTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    int16_t minX = std::min({x0, x1, x2});
    int16_t maxX = std::max({x0, x1, x2});
    int16_t minY = std::min({y0, y1, y2});
    int16_t maxY = std::max({y0, y1, y2});

    minX = std::max<int16_t>(minX, 0);
    minY = std::max<int16_t>(minY, 0);
    maxX = std::min<int16_t>(maxX, static_cast<int16_t>(VRAM::WIDTH - 1));
    maxY = std::min<int16_t>(maxY, static_cast<int16_t>(VRAM::HEIGHT - 1));

    for (int16_t py = minY; py <= maxY; py++) {
        for (int16_t px = minX; px <= maxX; px++) {
            if (IsInsideTriangle(px, py, x0, y0, x1, y1, x2, y2)) {
                PlotPixel(px, py, color);
            }
        }
    }
}

void Rasterizer::DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    DrawFlatTriangle(x0, y0, x1, y1, x2, y2, color);
}

void Rasterizer::DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    int16_t dx = static_cast<int16_t>(std::abs(x1 - x0));
    int16_t dy = static_cast<int16_t>(std::abs(y1 - y0));
    int16_t sx = x0 < x1 ? 1 : -1;
    int16_t sy = y0 < y1 ? 1 : -1;
    int16_t err = dx - dy;

    while (true) {
        PlotPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int16_t e2 = static_cast<int16_t>(2 * err);
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }
}

}