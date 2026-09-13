#pragma once
#include <cstdint>
#include <array>

namespace nova::ps1 {

class VRAM {
public:
    VRAM();
    ~VRAM() = default;

    void Reset();

    uint16_t Read16(uint32_t x, uint32_t y) const;
    void Write16(uint32_t x, uint32_t y, uint16_t value);

    uint8_t* GetRawData() { return _data.data(); }
    const uint8_t* GetRawData() const { return _data.data(); }

    static constexpr uint32_t WIDTH = 1024;
    static constexpr uint32_t HEIGHT = 512;
    static constexpr uint32_t SIZE = WIDTH * HEIGHT * 2;

private:
    std::array<uint8_t, SIZE> _data;

    uint32_t GetOffset(uint32_t x, uint32_t y) const;
};

}