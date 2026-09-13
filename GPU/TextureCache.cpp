#include "TextureCache.hpp"
#include "Texture.hpp"

namespace nova::ps1 {

TextureCache::TextureCache() : _texture(nullptr) {
}

void TextureCache::Reset() {
    _cache.clear();
}

uint64_t TextureCache::MakeKey(uint32_t u, uint32_t v) const {
    return (static_cast<uint64_t>(u) << 32) | static_cast<uint64_t>(v);
}

uint16_t TextureCache::Sample(uint32_t u, uint32_t v) {
    if (!_texture) return 0;

    uint64_t key = MakeKey(u, v);

    auto it = _cache.find(key);
    if (it != _cache.end()) {
        return it->second;
    }

    uint16_t value = _texture->Sample(u, v);
    _cache[key] = value;

    return value;
}

void TextureCache::Invalidate() {
    _cache.clear();
}

}