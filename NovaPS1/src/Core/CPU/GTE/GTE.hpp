#pragma once
#include <cstdint>
#include <array>

namespace nova::ps1 {

class GTE {
public:
    GTE();
    ~GTE() = default;

    void Reset();

    uint32_t ReadData(uint32_t reg) const;
    void WriteData(uint32_t reg, uint32_t value);

    uint32_t ReadControl(uint32_t reg) const;
    void WriteControl(uint32_t reg, uint32_t value);

    void Execute(uint32_t opcode);

private:
    std::array<uint32_t, 32> _data;
    std::array<uint32_t, 32> _control;

    int16_t GetIR(int index) const { return static_cast<int16_t>(_data[8 + index] & 0xFFFF); }
    void SetIR(int index, int16_t value) { _data[8 + index] = static_cast<uint32_t>(static_cast<uint16_t>(value)); }

    int16_t GetMAC(int index) const { return static_cast<int16_t>(_data[24 + index] & 0xFFFF); }
    void SetMAC(int index, int32_t value);

    uint32_t GetVXY(int index) const { return _data[0 + index]; }
    void SetVXY(int index, uint32_t value) { _data[0 + index] = value; }
    void SetVZ(int index, uint16_t value) { _data[4 + index] = value; }

    uint32_t GetOTZ() const { return _data[7] & 0xFFFF; }
    void SetOTZ(uint16_t value) { _data[7] = value; }

    uint32_t GetIR0() const { return _data[8]; }
    void SetIR0(uint32_t value) { _data[8] = value; }

    void SetMAC0(int32_t value);
    int32_t GetMAC0() const { return static_cast<int32_t>(_data[24]); }

    void SetFlag(uint32_t bit) { _control[31] |= (1u << bit); }
    void ClearFlag(uint32_t bit) { _control[31] &= ~(1u << bit); }
    bool GetFlag(uint32_t bit) const { return (_control[31] & (1u << bit)) != 0; }

    int64_t MAC1, MAC2, MAC3;
    int64_t IR1, IR2, IR3;

    void PushSaturationFlags(int64_t mac1, int64_t mac2, int64_t mac3);
    int32_t SaturateMAC(int64_t value, uint32_t negativeBit, uint32_t positiveBit);
    int16_t SaturateIR(int64_t value, uint32_t bit);
    int16_t SaturateColor(int64_t value, uint32_t bit);
};

}