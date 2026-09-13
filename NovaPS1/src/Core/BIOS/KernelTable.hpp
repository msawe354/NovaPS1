#pragma once
#include <cstdint>
#include <array>

namespace nova::ps1 {

class KernelTable {
public:
    KernelTable();
    ~KernelTable() = default;

    void Reset();

    void SetEntry(uint32_t index, uint32_t address, uint32_t type);
    uint32_t GetAddress(uint32_t index) const;

    static constexpr uint32_t ENTRY_COUNT = 256;

    struct Entry {
        uint32_t address;
        uint32_t type;
    };

private:
    std::array<Entry, ENTRY_COUNT> _entries;
};

}