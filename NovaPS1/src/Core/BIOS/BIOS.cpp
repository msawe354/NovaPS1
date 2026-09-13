#include "BIOS.hpp"
#include "../Bus/Bus.hpp"
#include "../CPU/MIPS.hpp"
#include "../CPU/Registers.hpp"
#include <fstream>
#include <iostream>
#include <cstring>

namespace nova::ps1 {

BIOS::BIOS()
    : _bus(nullptr),
      _cpu(nullptr),
      _loaded(false) {
    _data.resize(BIOS_SIZE, 0);
}

void BIOS::Reset() {
    _loaded = false;
}

bool BIOS::Load(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "[BIOS] Cannot open: " << path << std::endl;
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size > BIOS_SIZE) {
        std::cerr << "[BIOS] File too large: " << size << " bytes" << std::endl;
        file.close();
        return false;
    }

    _data.resize(BIOS_SIZE, 0);
    file.read(reinterpret_cast<char*>(_data.data()), size);
    file.close();

    _loaded = true;
    std::cout << "[BIOS] Loaded: " << path << " (" << size << " bytes)" << std::endl;
    return true;
}

uint8_t BIOS::Read8(uint32_t offset) const {
    if (offset >= BIOS_SIZE) return 0;
    return _data[offset];
}

uint16_t BIOS::Read16(uint32_t offset) const {
    if (offset + 1 >= BIOS_SIZE) return 0;
    return static_cast<uint16_t>(_data[offset] | (_data[offset + 1] << 8));
}

uint32_t BIOS::Read32(uint32_t offset) const {
    if (offset + 3 >= BIOS_SIZE) return 0;
    return static_cast<uint32_t>(_data[offset])
         | (static_cast<uint32_t>(_data[offset + 1]) << 8)
         | (static_cast<uint32_t>(_data[offset + 2]) << 16)
         | (static_cast<uint32_t>(_data[offset + 3]) << 24);
}

void BIOS::HLE_PutChar(MIPS& cpu) {
    Registers& regs = cpu.GetRegisters();
    uint32_t ch = regs.GetGPR(4) & 0xFF;
    std::cout << static_cast<char>(ch);
    regs.SetGPR(2, 0);
}

void BIOS::HLE_PutString(MIPS& cpu) {
    Registers& regs = cpu.GetRegisters();
    uint32_t address = regs.GetGPR(4);
    uint32_t length = regs.GetGPR(5);

    if (_bus) {
        for (uint32_t i = 0; i < length; i++) {
            uint8_t ch = _bus->Read8(address + i);
            std::cout << static_cast<char>(ch);
        }
    }

    regs.SetGPR(2, 0);
}

void BIOS::HLE_Exit(MIPS& cpu) {
    Registers& regs = cpu.GetRegisters();
    std::cout << "\n[BIOS] Program exited with code: " << regs.GetGPR(4) << std::endl;
    regs.SetGPR(2, 0);
}

void BIOS::HLE_LoadExe(MIPS& cpu) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(2, 1);
}

void BIOS::HLE_FlushCache(MIPS& cpu) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(2, 0);
}

void BIOS::SyscallA(MIPS& cpu) {
    Registers& regs = cpu.GetRegisters();
    uint32_t function = regs.GetGPR(9);

    switch (function) {
        case 0x01: HLE_FlushCache(cpu); break;
        case 0x02: HLE_FlushCache(cpu); break;
        case 0x3C: HLE_PutChar(cpu); break;
        case 0x3E: HLE_PutString(cpu); break;
        case 0x00: HLE_LoadExe(cpu); break;
        default:
            regs.SetGPR(2, 0);
            break;
    }
}

void BIOS::SyscallB(MIPS& cpu) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(2, 0);
}

void BIOS::SyscallC(MIPS& cpu) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(2, 0);
}

void BIOS::HandleSyscall(MIPS& cpu, uint32_t code) {
    switch (code) {
        case 0xA0: SyscallA(cpu); break;
        case 0xB0: SyscallB(cpu); break;
        case 0xC0: SyscallC(cpu); break;
        default:
            cpu.GetRegisters().SetGPR(2, 0);
            break;
    }
}

}