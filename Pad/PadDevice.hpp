#pragma once
#include "Pad.hpp"
#include "../Bus/IBusDevice.hpp"

namespace nova::ps1 {

class PadDevice : public IBusDevice {
public:
    PadDevice();
    ~PadDevice() override = default;

    void Reset();

    uint32_t Read(uint32_t offset, unsigned size) override;
    void Write(uint32_t offset, uint32_t value, unsigned size) override;

    Pad& GetPad() { return _pad; }
    const Pad& GetPad() const { return _pad; }

private:
    Pad _pad;
};

}