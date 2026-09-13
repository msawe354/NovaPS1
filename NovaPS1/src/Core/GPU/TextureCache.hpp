#pragma once
#include <cstdint>
#include <unordered_map>

namespace nova::ps1 {

class Texture;

class TextureCache {
public:
    TextureCache();
    ~TextureCache() = default;

    void Reset();

    void SetTexture(Texture* texture) { _texture = texture; }

    uint16_t Sample(uint32_t u, uint32_t v);
    void Invalidate();

private:
    Texture* _texture;
    std::unordered_map<uint64_t, uint16_t> _cache;

    uint64_t MakeKey(uint32_t u, uint32_t v) const;
};

}