#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace nova::ps1 {

class Emulator;

class SaveState {
public:
    SaveState();
    ~SaveState() = default;

    void Reset();

    bool Save(Emulator& emu, const std::string& path);
    bool Load(Emulator& emu, const std::string& path);

    bool SaveToBuffer(Emulator& emu);
    bool LoadFromBuffer(Emulator& emu);

    const std::vector<uint8_t>& GetBuffer() const { return _buffer; }

    static constexpr uint32_t MAGIC = 0x4E4F5641;
    static constexpr uint32_t VERSION = 1;

private:
    std::vector<uint8_t> _buffer;

    void WriteU8(uint8_t value);
    void WriteU16(uint16_t value);
    void WriteU32(uint32_t value);
    void WriteU64(uint64_t value);
    void WriteBytes(const uint8_t* data, size_t size);

    uint8_t ReadU8();
    uint16_t ReadU16();
    uint32_t ReadU32();
    uint64_t ReadU64();
    void ReadBytes(uint8_t* data, size_t size);

    size_t _readPos;

    void SerializeCPU(Emulator& emu);
    void SerializeGPU(Emulator& emu);
    void SerializeSPU(Emulator& emu);
    void SerializeDMA(Emulator& emu);
    void SerializeTimers(Emulator& emu);
    void SerializeCDROM(Emulator& emu);
    void SerializePad(Emulator& emu);

    void DeserializeCPU(Emulator& emu);
    void DeserializeGPU(Emulator& emu);
    void DeserializeSPU(Emulator& emu);
    void DeserializeDMA(Emulator& emu);
    void DeserializeTimers(Emulator& emu);
    void DeserializeCDROM(Emulator& emu);
    void DeserializePad(Emulator& emu);
};

}