#include "SaveState.hpp"
#include "../Emulator.hpp"
#include "../CPU/MIPS.hpp"
#include "../CPU/Registers.hpp"
#include "../GPU/GPU.hpp"
#include "../GPU/VRAM.hpp"
#include "../SPU/SPU.hpp"
#include "../DMA/DMA.hpp"
#include "../Timers/Timers.hpp"
#include "../CDROM/CDROM.hpp"
#include "../Pad/Pad.hpp"
#include <fstream>
#include <cstring>

namespace nova::ps1 {

SaveState::SaveState() : _readPos(0) {
}

void SaveState::Reset() {
    _buffer.clear();
    _readPos = 0;
}

void SaveState::WriteU8(uint8_t value) {
    _buffer.push_back(value);
}

void SaveState::WriteU16(uint16_t value) {
    _buffer.push_back(static_cast<uint8_t>(value & 0xFF));
    _buffer.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
}

void SaveState::WriteU32(uint32_t value) {
    _buffer.push_back(static_cast<uint8_t>(value & 0xFF));
    _buffer.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
    _buffer.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
    _buffer.push_back(static_cast<uint8_t>((value >> 24) & 0xFF));
}

void SaveState::WriteU64(uint64_t value) {
    for (int i = 0; i < 8; i++) {
        _buffer.push_back(static_cast<uint8_t>((value >> (i * 8)) & 0xFF));
    }
}

void SaveState::WriteBytes(const uint8_t* data, size_t size) {
    _buffer.insert(_buffer.end(), data, data + size);
}

uint8_t SaveState::ReadU8() {
    if (_readPos >= _buffer.size()) return 0;
    return _buffer[_readPos++];
}

uint16_t SaveState::ReadU16() {
    uint16_t lo = ReadU8();
    uint16_t hi = ReadU8();
    return static_cast<uint16_t>(lo | (hi << 8));
}

uint32_t SaveState::ReadU32() {
    uint32_t b0 = ReadU8();
    uint32_t b1 = ReadU8();
    uint32_t b2 = ReadU8();
    uint32_t b3 = ReadU8();
    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}

uint64_t SaveState::ReadU64() {
    uint64_t value = 0;
    for (int i = 0; i < 8; i++) {
        value |= static_cast<uint64_t>(ReadU8()) << (i * 8);
    }
    return value;
}

void SaveState::ReadBytes(uint8_t* data, size_t size) {
    if (_readPos + size > _buffer.size()) return;
    std::memcpy(data, &_buffer[_readPos], size);
    _readPos += size;
}

void SaveState::SerializeCPU(Emulator& emu) {
    Registers& regs = emu.GetCPU().GetRegisters();

    for (uint32_t i = 0; i < 32; i++) {
        WriteU32(regs.GetGPR(i));
    }

    WriteU32(regs.GetHI());
    WriteU32(regs.GetLO());
    WriteU32(regs.GetPC());
    WriteU32(regs.GetNextPC());

    for (uint32_t i = 0; i < 32; i++) {
        WriteU32(regs.GetCOP0(i));
    }

    for (uint32_t i = 0; i < 32; i++) {
        WriteU32(regs.GetCOP1(i));
    }

    WriteU32(regs.GetFCR31());

    for (uint32_t i = 0; i < 32; i++) {
        WriteU32(regs.GetCOP2(i));
    }

    for (uint32_t i = 0; i < 32; i++) {
        WriteU32(regs.GetCOP2Control(i));
    }

    WriteU64(emu.GetCPU().GetCycleCount());
}

void SaveState::SerializeGPU(Emulator& emu) {
    VRAM& vram = emu.GetGPU().GetVRAM();
    const uint8_t* vramData = vram.GetRawData();
    WriteBytes(vramData, VRAM::SIZE);
}

void SaveState::SerializeSPU(Emulator& emu) {
    WriteU32(0);
}

void SaveState::SerializeDMA(Emulator& emu) {
    for (uint32_t i = 0; i < DMA::CHANNEL_COUNT; i++) {
        WriteU32(0);
        WriteU32(0);
        WriteU32(0);
    }
}

void SaveState::SerializeTimers(Emulator& emu) {
    for (uint32_t i = 0; i < Timers::TIMER_COUNT; i++) {
        WriteU16(0);
        WriteU16(0);
        WriteU16(0);
    }
}

void SaveState::SerializeCDROM(Emulator& emu) {
    WriteU8(0);
    WriteU8(0);
    WriteU8(0);
}

void SaveState::SerializePad(Emulator& emu) {
    WriteU16(emu.GetPad().GetButtonState(0));
    WriteU16(emu.GetPad().GetButtonState(1));
}

void SaveState::DeserializeCPU(Emulator& emu) {
    Registers& regs = emu.GetCPU().GetRegisters();

    for (uint32_t i = 0; i < 32; i++) {
        regs.SetGPR(i, ReadU32());
    }

    regs.SetHI(ReadU32());
    regs.SetLO(ReadU32());
    regs.SetPC(ReadU32());
    regs.SetNextPC(ReadU32());

    for (uint32_t i = 0; i < 32; i++) {
        regs.SetCOP0(i, ReadU32());
    }

    for (uint32_t i = 0; i < 32; i++) {
        regs.SetCOP1(i, ReadU32());
    }

    regs.SetFCR31(ReadU32());

    for (uint32_t i = 0; i < 32; i++) {
        regs.SetCOP2(i, ReadU32());
    }

    for (uint32_t i = 0; i < 32; i++) {
        regs.SetCOP2Control(i, ReadU32());
    }

    ReadU64();
}

void SaveState::DeserializeGPU(Emulator& emu) {
    VRAM& vram = emu.GetGPU().GetVRAM();
    uint8_t* vramData = vram.GetRawData();
    ReadBytes(vramData, VRAM::SIZE);
}

void SaveState::DeserializeSPU(Emulator& emu) {
    ReadU32();
}

void SaveState::DeserializeDMA(Emulator& emu) {
    for (uint32_t i = 0; i < DMA::CHANNEL_COUNT; i++) {
        ReadU32();
        ReadU32();
        ReadU32();
    }
}

void SaveState::DeserializeTimers(Emulator& emu) {
    for (uint32_t i = 0; i < Timers::TIMER_COUNT; i++) {
        ReadU16();
        ReadU16();
        ReadU16();
    }
}

void SaveState::DeserializeCDROM(Emulator& emu) {
    ReadU8();
    ReadU8();
    ReadU8();
}

void SaveState::DeserializePad(Emulator& emu) {
    uint16_t port0 = ReadU16();
    uint16_t port1 = ReadU16();
    emu.GetPad().SetButtonState(0, port0);
    emu.GetPad().SetButtonState(1, port1);
}

bool SaveState::SaveToBuffer(Emulator& emu) {
    _buffer.clear();

    WriteU32(MAGIC);
    WriteU32(VERSION);

    SerializeCPU(emu);
    SerializeGPU(emu);
    SerializeSPU(emu);
    SerializeDMA(emu);
    SerializeTimers(emu);
    SerializeCDROM(emu);
    SerializePad(emu);

    WriteU32(0xDEADBEEF);

    return true;
}

bool SaveState::LoadFromBuffer(Emulator& emu) {
    _readPos = 0;

    uint32_t magic = ReadU32();
    if (magic != MAGIC) return false;

    uint32_t version = ReadU32();
    if (version != VERSION) return false;

    DeserializeCPU(emu);
    DeserializeGPU(emu);
    DeserializeSPU(emu);
    DeserializeDMA(emu);
    DeserializeTimers(emu);
    DeserializeCDROM(emu);
    DeserializePad(emu);

    uint32_t marker = ReadU32();
    if (marker != 0xDEADBEEF) return false;

    return true;
}

bool SaveState::Save(Emulator& emu, const std::string& path) {
    if (!SaveToBuffer(emu)) return false;

    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) return false;

    file.write(reinterpret_cast<const char*>(_buffer.data()), _buffer.size());
    file.close();

    return true;
}

bool SaveState::Load(Emulator& emu, const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    _buffer.resize(static_cast<size_t>(size));
    file.read(reinterpret_cast<char*>(_buffer.data()), size);
    file.close();

    return LoadFromBuffer(emu);
}

}