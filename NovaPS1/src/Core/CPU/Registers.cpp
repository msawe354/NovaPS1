#include "Registers.hpp"

namespace nova::ps1 {

Registers::Registers()
    : _hi(0), _lo(0), _pc(0), _nextPc(0), _fcr31(0) {
    Reset();
}

void Registers::Reset() {
    _gpr.fill(0);
    _cop0.fill(0);
    _cop1.fill(0);
    _cop2.fill(0);
    _cop2Control.fill(0);

    _hi = 0;
    _lo = 0;
    _pc = 0xBFC00000;
    _nextPc = _pc + 4;
    _fcr31 = 0;
}

uint32_t Registers::GetGPR(uint32_t index) const {
    if (index == 0) return 0;
    return _gpr[index & 0x1F];
}

void Registers::SetGPR(uint32_t index, uint32_t value) {
    if (index == 0) return;
    _gpr[index & 0x1F] = value;
}

uint32_t Registers::GetCOP0(uint32_t index) const {
    return _cop0[index & 0x1F];
}

void Registers::SetCOP0(uint32_t index, uint32_t value) {
    _cop0[index & 0x1F] = value;
}

uint32_t Registers::GetCOP1(uint32_t index) const {
    return _cop1[index & 0x1F];
}

void Registers::SetCOP1(uint32_t index, uint32_t value) {
    _cop1[index & 0x1F] = value;
}

uint64_t Registers::GetCOP1D(uint32_t index) const {
    uint32_t lo = _cop1[index & 0x1F];
    uint32_t hi = _cop1[(index + 1) & 0x1F];
    return static_cast<uint64_t>(lo) | (static_cast<uint64_t>(hi) << 32);
}

void Registers::SetCOP1D(uint32_t index, uint64_t value) {
    _cop1[index & 0x1F] = static_cast<uint32_t>(value & 0xFFFFFFFF);
    _cop1[(index + 1) & 0x1F] = static_cast<uint32_t>((value >> 32) & 0xFFFFFFFF);
}

uint32_t Registers::GetCOP2(uint32_t index) const {
    return _cop2[index & 0x1F];
}

void Registers::SetCOP2(uint32_t index, uint32_t value) {
    _cop2[index & 0x1F] = value;
}

uint32_t Registers::GetCOP2Control(uint32_t index) const {
    return _cop2Control[index & 0x1F];
}

void Registers::SetCOP2Control(uint32_t index, uint32_t value) {
    _cop2Control[index & 0x1F] = value;
}

}