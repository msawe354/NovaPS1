#pragma once
#include <cstdint>

namespace nova::ps1 {

class DMAControl {
public:
    DMAControl();
    ~DMAControl() = default;

    void Reset();

    uint32_t ReadControl() const { return _control; }
    void WriteControl(uint32_t value) { _control = value; }

    bool IsChannelEnabled(uint32_t channel) const;

    void EnableChannel(uint32_t channel);
    void DisableChannel(uint32_t channel);

private:
    uint32_t _control;
};

}