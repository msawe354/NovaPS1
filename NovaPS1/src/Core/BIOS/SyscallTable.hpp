#pragma once
#include <cstdint>
#include <array>
#include <string>

namespace nova::ps1 {

class SyscallTable {
public:
    SyscallTable();
    ~SyscallTable() = default;

    void Reset();

    void Register(uint8_t code, const std::string& name);
    std::string GetName(uint8_t code) const;
    bool IsRegistered(uint8_t code) const;

    static constexpr uint32_t TABLE_SIZE = 256;

private:
    std::array<std::string, TABLE_SIZE> _names;
    std::array<bool, TABLE_SIZE> _registered;
};

}