#pragma once
#include <cstdint>
#include <array>

namespace nova::ps1 {

class Registers {
public:
    Registers();
    ~Registers() = default;

    void Reset();

    uint32_t GetGPR(uint32_t index) const;
    void SetGPR(uint32_t index, uint32_t value);

    uint32_t GetHI() const { return _hi; }
    void SetHI(uint32_t value) { _hi = value; }

    uint32_t GetLO() const { return _lo; }
    void SetLO(uint32_t value) { _lo = value; }

    uint32_t GetPC() const { return _pc; }
    void SetPC(uint32_t value) { _pc = value; }

    uint32_t GetNextPC() const { return _nextPc; }
    void SetNextPC(uint32_t value) { _nextPc = value; }

    uint32_t GetCOP0(uint32_t index) const;
    void SetCOP0(uint32_t index, uint32_t value);

    uint32_t GetCOP1(uint32_t index) const;
    void SetCOP1(uint32_t index, uint32_t value);

    uint64_t GetCOP1D(uint32_t index) const;
    void SetCOP1D(uint32_t index, uint64_t value);

    uint32_t GetFCR31() const { return _fcr31; }
    void SetFCR31(uint32_t value) { _fcr31 = value; }

    uint32_t GetCOP2(uint32_t index) const;
    void SetCOP2(uint32_t index, uint32_t value);

    uint32_t GetCOP2Control(uint32_t index) const;
    void SetCOP2Control(uint32_t index, uint32_t value);

private:
    std::array<uint32_t, 32> _gpr;
    std::array<uint32_t, 32> _cop0;
    std::array<uint32_t, 32> _cop1;
    std::array<uint32_t, 32> _cop2;
    std::array<uint32_t, 32> _cop2Control;

    uint32_t _hi;
    uint32_t _lo;
    uint32_t _pc;
    uint32_t _nextPc;
    uint32_t _fcr31;
};

}