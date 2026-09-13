#pragma once
#include <cstdint>
#include <array>

namespace nova::ps1 {

class Cache {
public:
    Cache();
    ~Cache() = default;

    void Reset();

    bool Read(uint32_t address, uint32_t& value);
    void Write(uint32_t address, uint32_t value);
    void Invalidate(uint32_t address);
    void InvalidateAll();

    static constexpr uint32_t LINE_SIZE = 16;
    static constexpr uint32_t LINE_COUNT = 256;
    static constexpr uint32_t TOTAL_SIZE = LINE_SIZE * LINE_COUNT;

private:
    struct Line {
        bool valid;
        uint32_t tag;
        std::array<uint8_t, LINE_SIZE> data;
    };

    std::array<Line, LINE_COUNT> _lines;

    uint32_t GetIndex(uint32_t address) const;
    uint32_t GetTag(uint32_t address) const;
    uint32_t GetOffset(uint32_t address) const;
};

}