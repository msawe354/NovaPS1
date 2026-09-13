#pragma once
#include <cstdint>
#include <array>

namespace nova::ps1 {

class TLB {
public:
    TLB();
    ~TLB() = default;

    void Reset();

    bool Probe(uint32_t virtualAddress, uint32_t& physicalAddress);
    void WriteEntry(uint32_t index, uint32_t hi, uint32_t lo0, uint32_t lo1);

    uint32_t ReadIndex() const { return _index; }
    void SetIndex(uint32_t value) { _index = value; }

    uint32_t ReadRandom() const;
    uint32_t ReadEntryHi(uint32_t index) const;
    uint32_t ReadEntryLo0(uint32_t index) const;
    uint32_t ReadEntryLo1(uint32_t index) const;

    static constexpr uint32_t ENTRY_COUNT = 64;

private:
    struct Entry {
        uint32_t hi;
        uint32_t lo0;
        uint32_t lo1;
    };

    std::array<Entry, ENTRY_COUNT> _entries;
    uint32_t _index;

    bool MatchEntry(const Entry& entry, uint32_t vpn) const;
};

}