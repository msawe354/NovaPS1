#include "DMAControl.hpp"

namespace nova::ps1 {

DMAControl::DMAControl() : _control(0) {
}

void DMAControl::Reset() {
    _control = 0;
}

bool DMAControl::IsChannelEnabled(uint32_t channel) const {
    if (channel >= 7) return false;
    return (_control & (1u << (channel * 4))) != 0;
}

void DMAControl::EnableChannel(uint32_t channel) {
    if (channel >= 7) return;
    _control |= (1u << (channel * 4));
}

void DMAControl::DisableChannel(uint32_t channel) {
    if (channel >= 7) return;
    _control &= ~(1u << (channel * 4));
}

}