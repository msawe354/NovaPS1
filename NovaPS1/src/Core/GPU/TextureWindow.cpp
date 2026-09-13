#include "TextureWindow.hpp"

namespace nova::ps1 {

TextureWindow::TextureWindow()
    : _maskX(0), _maskY(0), _offsetX(0), _offsetY(0) {
}

void TextureWindow::Reset() {
    _maskX = 0;
    _maskY = 0;
    _offsetX = 0;
    _offsetY = 0;
}

void TextureWindow::Set(uint32_t value) {
    _maskX   = (value & 0x1F);
    _maskY   = ((value >> 5) & 0x1F);
    _offsetX = ((value >> 10) & 0x1F);
    _offsetY = ((value >> 15) & 0x1F);
}

uint32_t TextureWindow::ApplyX(uint32_t x) const {
    if (_maskX == 0) return x;
    return (x & ~(_maskX * 8)) | ((_offsetX & _maskX) * 8);
}

uint32_t TextureWindow::ApplyY(uint32_t y) const {
    if (_maskY == 0) return y;
    return (y & ~(_maskY * 8)) | ((_offsetY & _maskY) * 8);
}

}