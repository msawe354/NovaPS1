#include "GTE.hpp"

namespace nova::ps1 {

GTE::GTE() : MAC1(0), MAC2(0), MAC3(0), IR1(0), IR2(0), IR3(0) {
    Reset();
}

void GTE::Reset() {
    _data.fill(0);
    _control.fill(0);
    MAC1 = MAC2 = MAC3 = 0;
    IR1 = IR2 = IR3 = 0;
}

uint32_t GTE::ReadData(uint32_t reg) const {
    if (reg >= 32) return 0;
    return _data[reg];
}

void GTE::WriteData(uint32_t reg, uint32_t value) {
    if (reg >= 32) return;
    _data[reg] = value;
}

uint32_t GTE::ReadControl(uint32_t reg) const {
    if (reg >= 32) return 0;
    return _control[reg];
}

void GTE::WriteControl(uint32_t reg, uint32_t value) {
    if (reg >= 32) return;
    _control[reg] = value;
}

void GTE::SetMAC(int index, int32_t value) {
    if (index < 0 || index > 3) return;
    _data[24 + index] = static_cast<uint32_t>(value);
    if (index == 1) MAC1 = value;
    if (index == 2) MAC2 = value;
    if (index == 3) MAC3 = value;
}

void GTE::SetMAC0(int32_t value) {
    _data[24] = static_cast<uint32_t>(value);
}

int32_t GTE::SaturateMAC(int64_t value, uint32_t negativeBit, uint32_t positiveBit) {
    if (value > 0x7FFFFFFFFFFLL) {
        SetFlag(positiveBit);
        return static_cast<int32_t>(0x7FFFFFFFFFFLL);
    }
    if (value < -0x80000000000LL) {
        SetFlag(negativeBit);
        return static_cast<int32_t>(-0x80000000000LL);
    }
    return static_cast<int32_t>(value);
}

int16_t GTE::SaturateIR(int64_t value, uint32_t bit) {
    if (value > 0x7FFF) {
        SetFlag(bit);
        return 0x7FFF;
    }
    if (value < -0x8000) {
        SetFlag(bit);
        return static_cast<int16_t>(-0x8000);
    }
    return static_cast<int16_t>(value);
}

int16_t GTE::SaturateColor(int64_t value, uint32_t bit) {
    if (value > 0xFF) {
        SetFlag(bit);
        return 0xFF;
    }
    if (value < 0) {
        SetFlag(bit);
        return 0;
    }
    return static_cast<int16_t>(value);
}

void GTE::PushSaturationFlags(int64_t mac1, int64_t mac2, int64_t mac3) {
    if (mac1 > 0x7FFFFFFFFFFLL) SetFlag(30);
    if (mac1 < -0x80000000000LL) SetFlag(27);
    if (mac2 > 0x7FFFFFFFFFFLL) SetFlag(29);
    if (mac2 < -0x80000000000LL) SetFlag(26);
    if (mac3 > 0x7FFFFFFFFFFLL) SetFlag(28);
    if (mac3 < -0x80000000000LL) SetFlag(25);
}

void GTE::Execute(uint32_t opcode) {
    uint32_t func = opcode & 0x3F;
    (void)func;
}

}