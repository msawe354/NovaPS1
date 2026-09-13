#include "Cache.hpp"
#include <cstring>

namespace nova::ps1 {

Cache::Cache() {
    Reset();
}

void Cache::Reset() {
    for (auto& line : _lines) {
        line.valid = false;
        line.tag = 0;
        line.data.fill(0);
    }
}

uint32_t Cache::GetIndex(uint32_t address) const {
    return (address >> 4) & (LINE_COUNT - 1);
}

uint32_t Cache::GetTag(uint32_t address) const {
    return address >> 12;
}

uint32_t Cache::GetOffset(uint32_t address) const {
    return address & 0xF;
}

bool Cache::Read(uint32_t address, uint32_t& value) {
    uint32_t index = GetIndex(address);
    uint32_t tag = GetTag(address);
    uint32_t offset = GetOffset(address);

    Line& line = _lines[index];

    if (!line.valid || line.tag != tag) {
        return false;
    }

    if (offset + 4 > LINE_SIZE) {
        return false;
    }

    std::memcpy(&value, &line.data[offset], 4);
    return true;
}

void Cache::Write(uint32_t address, uint32_t value) {
    uint32_t index = GetIndex(address);
    uint32_t tag = GetTag(address);
    uint32_t offset = GetOffset(address);

    Line& line = _lines[index];

    if (!line.valid || line.tag != tag) {
        line.valid = true;
        line.tag = tag;
        line.data.fill(0);
    }

    if (offset + 4 <= LINE_SIZE) {
        std::memcpy(&line.data[offset], &value, 4);
    }
}

void Cache::Invalidate(uint32_t address) {
    uint32_t index = GetIndex(address);
    _lines[index].valid = false;
}

void Cache::InvalidateAll() {
    for (auto& line : _lines) {
        line.valid = false;
    }
}

}