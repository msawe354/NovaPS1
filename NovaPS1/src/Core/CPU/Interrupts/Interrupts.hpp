#pragma once
#include <cstdint>
#include <functional>
#include <array>

namespace nova::ps1 {

class Interrupts {
public:
    Interrupts();
    ~Interrupts() = default;

    void Reset();

    void Request(uint32_t line);
    void Clear(uint32_t line);

    bool IsPending(uint32_t line) const;
    bool HasPending() const;

    uint32_t ReadStatus() const { return _status; }
    uint32_t ReadMask() const { return _mask; }
    uint32_t ReadPending() const { return _pending & _mask; }

    void WriteStatus(uint32_t value);
    void WriteMask(uint32_t value);

    static constexpr uint32_t IRQ_VBLANK  = 0x000;
    static constexpr uint32_t IRQ_GPU     = 0x001;
    static constexpr uint32_t IRQ_CDROM   = 0x002;
    static constexpr uint32_t IRQ_DMA     = 0x003;
    static constexpr uint32_t IRQ_TIMER0  = 0x004;
    static constexpr uint32_t IRQ_TIMER1  = 0x005;
    static constexpr uint32_t IRQ_TIMER2  = 0x006;
    static constexpr uint32_t IRQ_CONTROLLER = 0x007;
    static constexpr uint32_t IRQ_SIO     = 0x008;
    static constexpr uint32_t IRQ_SPU     = 0x009;
    static constexpr uint32_t IRQ_PIO     = 0x00A;

private:
    uint32_t _status;
    uint32_t _mask;
    uint32_t _pending;
};

}