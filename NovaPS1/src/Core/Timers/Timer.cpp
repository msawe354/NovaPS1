#include "Timer.hpp"

namespace nova::ps1 {

Timer::Timer()
    : _counter(0), _target(0), _mode(0),
      _prescaler(1), _prescalerCounter(0),
      _irqOnTarget(false), _irqOnOverflow(false),
      _irqRepeat(false), _irqPulse(false),
      _reachedTarget(false), _overflowed(false) {
}

void Timer::Reset() {
    _counter = 0;
    _target = 0;
    _mode = 0;
    _prescaler = 1;
    _prescalerCounter = 0;
    _irqOnTarget = false;
    _irqOnOverflow = false;
    _irqRepeat = false;
    _irqPulse = false;
    _reachedTarget = false;
    _overflowed = false;
}

void Timer::Tick(uint32_t cycles) {
    _prescalerCounter += cycles;

    while (_prescalerCounter >= _prescaler) {
        _prescalerCounter -= _prescaler;

        _counter++;

        if (_counter == _target) {
            _reachedTarget = true;
        }

        if (_counter == 0xFFFF) {
            _overflowed = true;
        }
    }
}

}