#pragma once
#include <cstdint>

namespace nova::ps1 {

class VRAM;

class Rasterizer {
public:
    Rasterizer();
    ~Rasterizer() = default;

    void Reset();

    void SetVRAM(VRAM* vram) { _vram = vram; }

    void DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    void DrawRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color);
    void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void DrawFlatTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

    void FillRect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color);

private:
    VRAM* _vram;

    void PlotPixel(int16_t x, int16_t y, uint16_t color);
    bool IsInsideTriangle(int16_t px, int16_t py,
                          int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                          int16_t x2, int16_t y2);
};

}