#include "Bus.hpp"
#include <cstring>

namespace nova::ps1 {

Bus::Bus() : _biosLoaded(false) {
    Reset();
}

void Bus::Reset() {
    _ram.fill(0);
    _scratchpad.fill(0);
    _bios.fill(0);
    _biosLoaded = false;
    _devices.clear();
}

uint32_t Bus::ResolveAddress(uint32_t address) const {
    if (address >= 0x80000000 && address < 0xC0000000) {
        return address & 0x1FFFFFFF;
    }
    if (address >= 0xA0000000 && address < 0xC0000000) {
        return address & 0x1FFFFFFF;
    }
    return address;
}

bool Bus::IsRam(uint32_t addr) const {
    return addr < 0x00800000;
}

bool Bus::IsScratchpad(uint32_t addr) const {
    return addr >= 0x1F800000 && addr < 0x1F800400;
}

bool Bus::IsBios(uint32_t addr) const {
    return addr >= 0x1FC00000 && addr < 0x1FC80000;
}

IBusDevice* Bus::FindDevice(uint32_t address, uint32_t& offset) {
    for (auto& dev : _devices) {
        if (address >= dev.base && address < dev.base + dev.size) {
            offset = address - dev.base;
            return dev.device;
        }
    }
    return nullptr;
}

uint8_t Bus::Read8(uint32_t address) {
    uint32_t phys = ResolveAddress(address);

    uint32_t offset = 0;
    IBusDevice* dev = FindDevice(phys, offset);
    if (dev) {
        return static_cast<uint8_t>(dev->Read(offset, 1) & 0xFF);
    }

    if (IsBios(phys)) {
        return _bios[phys - 0x1FC00000];
    }
    if (IsRam(phys)) {
        return _ram[phys & 0x1FFFFF];
    }
    if (IsScratchpad(phys)) {
        return _scratchpad[phys - 0x1F800000];
    }

    return 0xFF;
}

uint16_t Bus::Read16(uint32_t address) {
    uint32_t phys = ResolveAddress(address);

    uint32_t offset = 0;
    IBusDevice* dev = FindDevice(phys, offset);
    if (dev) {
        return static_cast<uint16_t>(dev->Read(offset, 2) & 0xFFFF);
    }

    uint16_t lo = Read8(phys);
    uint16_t hi = Read8(phys + 1);
    return static_cast<uint16_t>(lo | (hi << 8));
}

uint32_t Bus::Read32(uint32_t address) {
    uint32_t phys = ResolveAddress(address);

    uint32_t offset = 0;
    IBusDevice* dev = FindDevice(phys, offset);
    if (dev) {
        return dev->Read(offset, 4);
    }

    uint32_t b0 = Read8(phys);
    uint32_t b1 = Read8(phys + 1);
    uint32_t b2 = Read8(phys + 2);
    uint32_t b3 = Read8(phys + 3);
    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}

void Bus::Write8(uint32_t address, uint8_t value) {
    uint32_t phys = ResolveAddress(address);

    uint32_t offset = 0;
    IBusDevice* dev = FindDevice(phys, offset);
    if (dev) {
        dev->Write(offset, value, 1);
        return;
    }

    if (IsRam(phys)) {
        _ram[phys & 0x1FFFFF] = value;
        return;
    }
    if (IsScratchpad(phys)) {
        _scratchpad[phys - 0x1F800000] = value;
        return;
    }
}

void Bus::Write16(uint32_t address, uint16_t value) {
    uint32_t phys = ResolveAddress(address);

    uint32_t offset = 0;
    IBusDevice* dev = FindDevice(phys, offset);
    if (dev) {
        dev->Write(offset, value, 2);
        return;
    }

    Write8(phys, static_cast<uint8_t>(value & 0xFF));
    Write8(phys + 1, static_cast<uint8_t>((value >> 8) & 0xFF));
}

void Bus::Write32(uint32_t address, uint32_t value) {
    uint32_t phys = ResolveAddress(address);

    uint32_t offset = 0;
    IBusDevice* dev = FindDevice(phys, offset);
    if (dev) {
        dev->Write(offset, value, 4);
        return;
    }

    Write8(phys, static_cast<uint8_t>(value & 0xFF));
    Write8(phys + 1, static_cast<uint8_t>((value >> 8) & 0xFF));
    Write8(phys + 2, static_cast<uint8_t>((value >> 16) & 0xFF));
    Write8(phys + 3, static_cast<uint8_t>((value >> 24) & 0xFF));
}

void Bus::LoadBios(const std::vector<uint8_t>& data) {
    size_t size = std::min(data.size(), _bios.size());
    std::memcpy(_bios.data(), data.data(), size);
    _biosLoaded = true;
}

void Bus::AttachDevice(uint32_t base, uint32_t size, IBusDevice* device) {
    _devices.push_back({ base, size, device, false });
}

void Bus::AttachMMIO(uint32_t base, uint32_t size, IBusDevice* device) {
    _devices.push_back({ base, size, device, true });
}

}