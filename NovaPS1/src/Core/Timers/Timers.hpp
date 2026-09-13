#pragma once
#include <cstdint>
#include <array>

namespace nova::ps1 {

class Interrupts;

class Timers {
public:
    Timers();
    ~Timers() = default;

    void Reset();
    void SetInterrupts(Interrupts* interrupts) { _interrupts = interrupts; }

    void Step(uint32_t cycles);

    uint32_t ReadRegister(uint32_t address);
    void WriteRegister(uint32_t address, uint32_t value);

    static constexpr uint32_t TIMER_COUNT = 3;

    enum TimerIndex : uint32_t {
        TIMER0 = 0,
        TIMER1 = 1,
        TIMER2 = 2
    };

private:
    struct Timer {
        uint16_t counter;
        uint16_t target;
        uint16_t mode;
        uint32_t clockSource;
        uint32_t prescaler;
        uint32_t prescalerCounter;
        bool irqOnTarget;
        bool irqOnOverflow;
        bool irqRepeat;
        bool irqPulse;
        bool reachedTarget;
        bool overflowed;
        bool irqPending;

        Timer()
            : counter(0), target(0), mode(0),
              clockSource(0), prescaler(1), prescalerCounter(0),
              irqOnTarget(false), irqOnOverflow(false),
              irqRepeat(false), irqPulse(false),
              reachedTarget(false), overflowed(false),
              irqPending(false) {}

        void Reset() {
            counter = 0;
            target = 0;
            mode = 0;
            clockSource = 0;
            prescaler = 1;
            prescalerCounter = 0;
            irqOnTarget = false;
            irqOnOverflow = false;
            irqRepeat = false;
            irqPulse = false;
            reachedTarget = false;
            overflowed = false;
            irqPending = false;
        }
    };

    std::array<Timer, TIMER_COUNT> _timers;
    Interrupts* _interrupts;

    uint32_t _systemClock;
    uint32_t _hblankCycles;

    static constexpr uint32_t SOURCE_SYSTEM_CLOCK = 0;
    static constexpr uint32_t SOURCE_DOTCLOCK = 1;
    static constexpr uint32_t SOURCE_HBLANK = 2;
    static constexpr uint32_t SOURCE_SYSTEM_CLOCK_8 = 3;

    void StepTimer(uint32_t index, uint32_t cycles);
    void UpdateMode(uint32_t index, uint32_t mode);
    void TriggerIRQ(uint32_t index);
    uint32_t GetPrescaler(uint32_t clockSource);
};

}