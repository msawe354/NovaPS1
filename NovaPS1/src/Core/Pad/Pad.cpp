#include "Pad.hpp"
#include "../CPU/Interrupts/Interrupts.hpp"

namespace nova::ps1 {

Pad::Pad()
    : _controlRegister(0),
      _baudRegister(0),
      _interrupts(nullptr),
      _transferCounter(0) {
    Reset();
}

void Pad::Reset() {
    for (auto& port : _ports) {
        port.Reset();
    }
    _controlRegister = 0;
    _baudRegister(0);
    _transferCounter = 0;
}

void Pad::PressButton(uint32_t port, Button button) {
    if (port >= PORT_COUNT) return;
    _ports[port].buttons &= ~static_cast<uint16_t>(button);
}

void Pad::ReleaseButton(uint32_t port, Button button) {
    if (port >= PORT_COUNT) return;
    _ports[port].buttons |= static_cast<uint16_t>(button);
}

void Pad::SetButtonState(uint32_t port, uint16_t state) {
    if (port >= PORT_COUNT) return;
    _ports[port].buttons = state;
}

uint16_t Pad::GetButtonState(uint32_t port) const {
    if (port >= PORT_COUNT) return 0xFFFF;
    return _ports[port].buttons;
}

void Pad::TransferBit(uint32_t port) {
    if (port >= PORT_COUNT) return;

    PadState& p = _ports[port];

    if (!p.transferActive) return;

    if (p.transferBit == 0) {
        p.transferData = p.buttons;
    }

    if (p.transferBit < 16) {
        p.transferData = static_cast<uint16_t>(p.transferData >> 1);
        p.transferBit++;
    } else {
        p.transferActive = false;
        p.transferBit = 0;
    }
}

void Pad::TriggerIRQ() {
    if (_interrupts) {
        _interrupts->Request(Interrupts::IRQ_CONTROLLER);
    }
}

void Pad::UpdateAck() {
    for (auto& port : _ports) {
        if (port.irqEnabled) {
            TriggerIRQ();
        }
    }
}

uint32_t Pad::ReadRegister(uint32_t address) {
    switch (address) {
        case 0x1F801040:
            if (_ports[0].transferActive) {
                uint16_t data = _ports[0].transferData & 0x01;
                TransferBit(0);
                return data;
            }
            return 0xFF;

        case 0x1F801044:
            return 0x00000005;

        case 0x1F801048:
            return _baudRegister;

        case 0x1F80104A:
            return _controlRegister;

        case 0x1F80104E:
            return 0;

        case 0x1F801050:
            if (_ports[1].transferActive) {
                uint16_t data = _ports[1].transferData & 0x01;
                TransferBit(1);
                return data;
            }
            return 0xFF;

        default:
            return 0;
    }
}

void Pad::WriteRegister(uint32_t address, uint32_t value) {
    switch (address) {
        case 0x1F801040:
            _ports[0].transferActive = true;
            _ports[0].transferBit = 0;
            _ports[0].transferData = 0;
            break;

        case 0x1F801048:
            _baudRegister = value & 0xFFFF;
            break;

        case 0x1F80104A:
            _controlRegister = value;
            for (auto& port : _ports) {
                port.irqEnabled = (value & 0x1000) != 0;
            }
            break;

        case 0x1F80104E:
            _ports[0].irqEnabled = false;
            break;

        case 0x1F801050:
            _ports[1].transferActive = true;
            _ports[1].transferBit = 0;
            _ports[1].transferData = 0;
            break;

        default:
            break;
    }
}

void Pad::Step(uint32_t cycles) {
    _transferCounter += cycles;

    if (_transferCounter >= 100) {
        _transferCounter = 0;
    }
}

}