#include "GP0.hpp"
#include "GPU.hpp"
#include "Texture.hpp"
#include "TextureCache.hpp"
#include "Color.hpp"
#include "TextureBlend.hpp"

namespace nova::ps1::gpu {

static Texture g_texture;
static TextureCache g_textureCache;
static TextureBlend g_textureBlend;

void SetTextureVRAM(VRAM* vram) {
    g_texture.SetVRAM(vram);
    g_textureCache.SetTexture(&g_texture);
}

void SetTexturePage(uint32_t value) {
    uint32_t pageX = (value & 0xF) * 64;
    uint32_t pageY = ((value >> 4) & 0x1) * 256;
    uint32_t depth = (value >> 7) & 0x3;

    g_texture.SetTexturePageX(pageX);
    g_texture.SetTexturePageY(pageY);
    g_texture.SetTextureDepth(depth);

    g_textureCache.Invalidate();
}

void SetCLUT(uint32_t value) {
    uint32_t clutX = (value & 0x3F) * 16;
    uint32_t clutY = ((value >> 6) & 0x1FF);

    g_texture.SetCLUTX(clutX);
    g_texture.SetCLUTY(clutY);

    g_textureCache.Invalidate();
}

void SetTextureBlendMode(uint32_t mode) {
    g_textureBlend.SetMode(mode);
}

Color SampleTexture(uint32_t u, uint32_t v) {
    uint16_t texel = g_textureCache.Sample(u, v);
    return Color::FromRGB15(texel);
}

Color BlendTextureWithVertex(const Color& texColor, const Color& vertexColor) {
    return g_textureBlend.Blend(texColor, vertexColor);
}

}