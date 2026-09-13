#pragma once
#include <cstdint>

namespace nova::ps1 {

class TimerControl {
public:
    TimerControl();
    ~TimerControl() = default;

    void Reset();

    void SetMode(uint32_t mode) { _mode = mode; }
    uint32_t GetMode() const { return _mode; }

    bool IsEnabled() const { return (_mode & 0x1000) != 0; }
    bool IsIRQOnTarget() const { return (_mode & 0x08) != 0; }
    bool IsIRQOnOverflow() const { return (_mode & 0x10) != 0; }
    bool IsIRQRepeat() const { return (_mode & 0x20) != 0; }
    bool IsIRQPulse() const { return (_mode & 0x40) != 0; }

    uint32_t GetClockSource() const { return _mode & 0x3; }

private:
    uint32_t _mode;
};

}