#pragma once
#include <cstdint>
#include <array>

namespace nova::ps1 {

class Interrupts;

class Pad {
public:
    Pad();
    ~Pad() = default;

    void Reset();
    void SetInterrupts(Interrupts* interrupts) { _interrupts = interrupts; }

    void Step(uint32_t cycles);

    uint32_t ReadRegister(uint32_t address);
    void WriteRegister(uint32_t address, uint32_t value);

    static constexpr uint32_t PORT_COUNT = 2;

    enum Button : uint16_t {
        Select   = 0x0001,
        L3       = 0x0002,
        R3       = 0x0004,
        Start    = 0x0008,
        Up       = 0x0010,
        Right    = 0x0020,
        Down     = 0x0040,
        Left     = 0x0080,
        L2       = 0x0100,
        R2       = 0x0200,
        L1       = 0x0400,
        R1       = 0x0800,
        Triangle = 0x1000,
        Circle   = 0x2000,
        Cross    = 0x4000,
        Square   = 0x8000
    };

    void PressButton(uint32_t port, Button button);
    void ReleaseButton(uint32_t port, Button button);
    void SetButtonState(uint32_t port, uint16_t state);

    uint16_t GetButtonState(uint32_t port) const;

private:
    struct PadState {
        uint16_t buttons;
        uint8_t status;
        bool connected;
        bool irqEnabled;
        uint16_t transferData;
        uint8_t transferBit;
        bool transferActive;

        PadState()
            : buttons(0xFFFF), status(0x5A), connected(true),
              irqEnabled(false), transferData(0),
              transferBit(0), transferActive(false) {}

        void Reset() {
            buttons = 0xFFFF;
            status = 0x5A;
            connected = true;
            irqEnabled = false;
            transferData = 0;
            transferBit = 0;
            transferActive = false;
        }
    };

    std::array<PadState, PORT_COUNT> _ports;

    uint32_t _controlRegister;
    uint32_t _baudRegister;
    Interrupts* _interrupts;

    uint32_t _transferCounter;

    void TransferBit(uint32_t port);
    void TriggerIRQ();
    void UpdateAck();
};

}