#include "Timers.hpp"
#include "../CPU/Interrupts/Interrupts.hpp"

namespace nova::ps1 {

Timers::Timers()
    : _interrupts(nullptr),
      _systemClock(0),
      _hblankCycles(0) {
    Reset();
}

void Timers::Reset() {
    for (auto& timer : _timers) {
        timer.Reset();
    }
    _systemClock = 0;
    _hblankCycles = 0;
}

uint32_t Timers::GetPrescaler(uint32_t clockSource) {
    switch (clockSource) {
        case SOURCE_SYSTEM_CLOCK: return 1;
        case SOURCE_DOTCLOCK: return 11;
        case SOURCE_HBLANK: return 1;
        case SOURCE_SYSTEM_CLOCK_8: return 8;
        default: return 1;
    }
}

void Timers::UpdateMode(uint32_t index, uint32_t mode) {
    if (index >= TIMER_COUNT) return;

    Timer& timer = _timers[index];
    timer.mode = static_cast<uint16_t>(mode);

    timer.clockSource = mode & 0x3;
    timer.irqOnTarget = (mode & 0x08) != 0;
    timer.irqOnOverflow = (mode & 0x10) != 0;
    timer.irqRepeat = (mode & 0x20) != 0;
    timer.irqPulse = (mode & 0x40) != 0;

    if (mode & 0x80) {
        timer.reachedTarget = true;
    } else {
        timer.reachedTarget = false;
    }

    if (mode & 0x100) {
        timer.overflowed = true;
    } else {
        timer.overflowed = false;
    }

    if (mode & 0x1000) {
        timer.counter = 0;
        timer.prescalerCounter = 0;
    }

    timer.prescaler = GetPrescaler(timer.clockSource);
}

void Timers::TriggerIRQ(uint32_t index) {
    if (!_interrupts) return;

    switch (index) {
        case TIMER0:
            _interrupts->Request(Interrupts::IRQ_TIMER0);
            break;
        case TIMER1:
            _interrupts->Request(Interrupts::IRQ_TIMER1);
            break;
        case TIMER2:
            _interrupts->Request(Interrupts::IRQ_TIMER2);
            break;
        default:
            break;
    }
}

void Timers::StepTimer(uint32_t index, uint32_t cycles) {
    if (index >= TIMER_COUNT) return;

    Timer& timer = _timers[index];

    if (timer.clockSource == SOURCE_HBLANK) {
        return;
    }

    timer.prescalerCounter += cycles;

    while (timer.prescalerCounter >= timer.prescaler) {
        timer.prescalerCounter -= timer.prescaler;

        timer.counter++;

        if (timer.counter == timer.target) {
            if (!timer.reachedTarget || timer.irqRepeat) {
                timer.reachedTarget = true;
                if (timer.irqOnTarget) {
                    TriggerIRQ(index);
                }
            }
        }

        if (timer.counter == 0xFFFF) {
            timer.overflowed = true;
            if (timer.irqOnOverflow) {
                TriggerIRQ(index);
            }
        }

        if (timer.counter == 0) {
            timer.counter = 0;
        }
    }
}

void Timers::Step(uint32_t cycles) {
    _systemClock += cycles;

    for (uint32_t i = 0; i < TIMER_COUNT; i++) {
        StepTimer(i, cycles);
    }
}

uint32_t Timers::ReadRegister(uint32_t address) {
    if (address < 0x1F801100 || address >= 0x1F801130) return 0;

    uint32_t timer = (address - 0x1F801100) / 0x10;
    uint32_t reg = ((address - 0x1F801100) % 0x10) / 4;

    if (timer >= TIMER_COUNT) return 0;

    Timer& t = _timers[timer];

    switch (reg) {
        case 0: return t.counter;
        case 1: return t.target;
        case 2: return t.mode;
        default: return 0;
    }
}

void Timers::WriteRegister(uint32_t address, uint32_t value) {
    if (address < 0x1F801100 || address >= 0x1F801130) return;

    uint32_t timer = (address - 0x1F801100) / 0x10;
    uint32_t reg = ((address - 0x1F801100) % 0x10) / 4;

    if (timer >= TIMER_COUNT) return;

    Timer& t = _timers[timer];

    switch (reg) {
        case 0:
            t.counter = static_cast<uint16_t>(value & 0xFFFF);
            break;
        case 1:
            t.target = static_cast<uint16_t>(value & 0xFFFF);
            break;
        case 2:
            UpdateMode(timer, value & 0x1FFF);
            break;
        default:
            break;
    }
}

}