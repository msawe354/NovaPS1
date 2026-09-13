#include "KernelTable.hpp"

namespace nova::ps1 {

KernelTable::KernelTable() {
    Reset();
}

void KernelTable::Reset() {
    for (auto& entry : _entries) {
        entry.address = 0;
        entry.type = 0;
    }
}

void KernelTable::SetEntry(uint32_t index, uint32_t address, uint32_t type) {
    if (index >= ENTRY_COUNT) return;
    _entries[index].address = address;
    _entries[index].type = type;
}

uint32_t KernelTable::GetAddress(uint32_t index) const {
    if (index >= ENTRY_COUNT) return 0;
    return _entries[index].address;
}

}