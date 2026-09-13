#include "BIOSHLE.hpp"
#include "../Bus/Bus.hpp"
#include "../CPU/MIPS.hpp"
#include "../CPU/Registers.hpp"
#include <iostream>

namespace nova::ps1 {

BIOSHLE::BIOSHLE() : _bus(nullptr) {
}

void BIOSHLE::Reset() {
}

std::string BIOSHLE::ReadString(uint32_t address, uint32_t maxLength) {
    std::string result;

    if (!_bus) return result;

    for (uint32_t i = 0; i < maxLength; i++) {
        uint8_t ch = _bus->Read8(address + i);
        if (ch == 0) break;
        result += static_cast<char>(ch);
    }

    return result;
}

void BIOSHLE::PrintString(const std::string& str) {
    std::cout << str;
}

void BIOSHLE::A0_FlushCache(MIPS& cpu) {
    cpu.GetRegisters().SetGPR(2, 0);
}

void BIOSHLE::A0_PutChar(MIPS& cpu) {
    Registers& regs = cpu.GetRegisters();
    uint32_t ch = regs.GetGPR(4) & 0xFF;
    std::cout << static_cast<char>(ch);
    regs.SetGPR(2, 0);
}

void BIOSHLE::A0_PutString(MIPS& cpu) {
    Registers& regs = cpu.GetRegisters();
    uint32_t address = regs.GetGPR(4);
    uint32_t length = regs.GetGPR(5);

    std::string str = ReadString(address, length);
    PrintString(str);

    regs.SetGPR(2, 0);
}

void BIOSHLE::A0_LoadExe(MIPS& cpu) {
    cpu.GetRegisters().SetGPR(2, 1);
}

void BIOSHLE::A0_Exit(MIPS& cpu) {
    Registers& regs = cpu.GetRegisters();
    std::cout << "\n[BIOS] Exited with code: " << regs.GetGPR(4) << std::endl;
    regs.SetGPR(2, 0);
}

void BIOSHLE::A0_OpenEvent(MIPS& cpu) {
    cpu.GetRegisters().SetGPR(2, 1);
}

void BIOSHLE::A0_CloseEvent(MIPS& cpu) {
    cpu.GetRegisters().SetGPR(2, 0);
}

void BIOSHLE::A0_WaitEvent(MIPS& cpu) {
    cpu.GetRegisters().SetGPR(2, 0);
}

void BIOSHLE::A0_TestEvent(MIPS& cpu) {
    cpu.GetRegisters().SetGPR(2, 0);
}

void BIOSHLE::A0_EnableEvent(MIPS& cpu) {
    cpu.GetRegisters().SetGPR(2, 0);
}

void BIOSHLE::A0_DisableEvent(MIPS& cpu) {
    cpu.GetRegisters().SetGPR(2, 0);
}

void BIOSHLE::A0_DeliverEvent(MIPS& cpu) {
    cpu.GetRegisters().SetGPR(2, 0);
}

void BIOSHLE::A0_GetLastError(MIPS& cpu) {
    cpu.GetRegisters().SetGPR(2, 0);
}

void BIOSHLE::HandleA0(MIPS& cpu, uint32_t function) {
    switch (function) {
        case 0x00: A0_LoadExe(cpu); break;
        case 0x01: A0_FlushCache(cpu); break;
        case 0x02: A0_FlushCache(cpu); break;
        case 0x03: A0_FlushCache(cpu); break;
        case 0x3C: A0_PutChar(cpu); break;
        case 0x3E: A0_PutString(cpu); break;
        case 0x3F: A0_Exit(cpu); break;
        case 0x08: A0_OpenEvent(cpu); break;
        case 0x09: A0_CloseEvent(cpu); break;
        case 0x0A: A0_WaitEvent(cpu); break;
        case 0x0B: A0_TestEvent(cpu); break;
        case 0x0C: A0_EnableEvent(cpu); break;
        case 0x0D: A0_DisableEvent(cpu); break;
        case 0x0E: A0_DeliverEvent(cpu); break;
        case 0x0F: A0_GetLastError(cpu); break;
        default:
            cpu.GetRegisters().SetGPR(2, 0);
            break;
    }
}

void BIOSHLE::HandleB0(MIPS& cpu, uint32_t function) {
    cpu.GetRegisters().SetGPR(2, 0);
}

void BIOSHLE::HandleC0(MIPS& cpu, uint32_t function) {
    cpu.GetRegisters().SetGPR(2, 0);
}

}