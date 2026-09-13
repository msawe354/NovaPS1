#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <functional>
#include "IBusDevice.hpp"

namespace nova::ps1 {

class Bus {
public:
    Bus();
    ~Bus() = default;

    void Reset();

    uint8_t  Read8(uint32_t address);
    uint16_t Read16(uint32_t address);
    uint32_t Read32(uint32_t address);

    void Write8(uint32_t address, uint8_t value);
    void Write16(uint32_t address, uint16_t value);
    void Write32(uint32_t address, uint32_t value);

    void LoadBios(const std::vector<uint8_t>& data);

    void AttachDevice(uint32_t base, uint32_t size, IBusDevice* device);
    void AttachMMIO(uint32_t base, uint32_t size, IBusDevice* device);

    uint32_t ResolveAddress(uint32_t address) const;

private:
    std::array<uint8_t, 0x200000> _ram;
    std::array<uint8_t, 0x400>    _scratchpad;
    std::array<uint8_t, 0x80000>  _bios;

    bool _biosLoaded;

    struct MappedDevice {
        uint32_t base;
        uint32_t size;
        IBusDevice* device;
        bool isMmio;
    };

    std::vector<MappedDevice> _devices;

    IBusDevice* FindDevice(uint32_t address, uint32_t& offset);

    bool IsRam(uint32_t addr) const;
    bool IsScratchpad(uint32_t addr) const;
    bool IsBios(uint32_t addr) const;
};

}