#include "SyscallTable.hpp"

namespace nova::ps1 {

SyscallTable::SyscallTable() {
    Reset();
}

void SyscallTable::Reset() {
    _names.fill("");
    _registered.fill(false);
}

void SyscallTable::Register(uint8_t code, const std::string& name) {
    _names[code] = name;
    _registered[code] = true;
}

std::string SyscallTable::GetName(uint8_t code) const {
    return _names[code];
}

bool SyscallTable::IsRegistered(uint8_t code) const {
    return _registered[code];
}

}