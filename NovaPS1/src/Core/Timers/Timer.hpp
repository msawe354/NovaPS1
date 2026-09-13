#pragma once
#include <cstdint>

namespace nova::ps1 {

class Timer {
public:
    Timer();
    ~Timer() = default;

    void Reset();

    void SetCounter(uint16_t value) { _counter = value; }
    void SetTarget(uint16_t value) { _target = value; }
    void SetMode(uint16_t value) { _mode = value; }

    uint16_t GetCounter() const { return _counter; }
    uint16_t GetTarget() const { return _target; }
    uint16_t GetMode() const { return _mode; }

    void Tick(uint32_t cycles);

    bool ReachedTarget() const { return _reachedTarget; }
    bool Overflowed() const { return _overflowed; }

private:
    uint16_t _counter;
    uint16_t _target;
    uint16_t _mode;

    uint32_t _prescaler;
    uint32_t _prescalerCounter;

    bool _irqOnTarget;
    bool _irqOnOverflow;
    bool _irqRepeat;
    bool _irqPulse;

    bool _reachedTarget;
    bool _overflowed;
};

}