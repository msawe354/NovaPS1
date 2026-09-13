#include "TLB.hpp"

namespace nova::ps1 {

TLB::TLB() : _index(0) {
    Reset();
}

void TLB::Reset() {
    for (auto& entry : _entries) {
        entry.hi = 0;
        entry.lo0 = 0;
        entry.lo1 = 0;
    }
    _index = 0;
}

bool TLB::MatchEntry(const Entry& entry, uint32_t vpn) const {
    uint32_t entryVpn = (entry.hi >> 13) & 0x7FFFF;
    uint32_t mask = (entry.hi & 0xFF) >> 1;
    uint32_t maskedVpn = vpn >> (13 - 0);
    return (maskedVpn & ~mask) == (entryVpn & ~mask);
}

bool TLB::Probe(uint32_t virtualAddress, uint32_t& physicalAddress) {
    uint32_t vpn = virtualAddress >> 12;
    bool odd = (vpn & 1) != 0;
    uint32_t baseVpn = vpn & ~1u;

    for (const auto& entry : _entries) {
        uint32_t entryVpn = (entry.hi >> 13) & 0x7FFFF;
        uint32_t mask = (entry.hi & 0xFF) >> 1;
        if ((baseVpn & ~mask) == (entryVpn & ~mask)) {
            uint32_t pfn = odd ? (entry.lo1 >> 6) : (entry.lo0 >> 6);
            physicalAddress = (pfn << 12) | (virtualAddress & 0xFFF);
            return true;
        }
    }

    physicalAddress = 0;
    return false;
}

void TLB::WriteEntry(uint32_t index, uint32_t hi, uint32_t lo0, uint32_t lo1) {
    if (index >= ENTRY_COUNT) return;
    _entries[index].hi = hi;
    _entries[index].lo0 = lo0;
    _entries[index].lo1 = lo1;
}

uint32_t TLB::ReadRandom() const {
    static uint32_t counter = 0;
    counter = (counter + 1) % ENTRY_COUNT;
    return counter;
}

uint32_t TLB::ReadEntryHi(uint32_t index) const {
    if (index >= ENTRY_COUNT) return 0;
    return _entries[index].hi;
}

uint32_t TLB::ReadEntryLo0(uint32_t index) const {
    if (index >= ENTRY_COUNT) return 0;
    return _entries[index].lo0;
}

uint32_t TLB::ReadEntryLo1(uint32_t index) const {
    if (index >= ENTRY_COUNT) return 0;
    return _entries[index].lo1;
}

}