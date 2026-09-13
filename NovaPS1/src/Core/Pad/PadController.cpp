#include "PadController.hpp"

namespace nova::ps1 {

PadController::PadController()
    : _pad(nullptr),
      _activePort(0) {
    Reset();
}

void PadController::Reset() {
    _mapper.Reset();
    _activePort = 0;
}

void PadController::OnKeyDown(uint32_t keyCode) {
    if (!_pad) return;
    _mapper.PressKey(*_pad, _activePort, keyCode);
}

void PadController::OnKeyUp(uint32_t keyCode) {
    if (!_pad) return;
    _mapper.ReleaseKey(*_pad, _activePort, keyCode);
}

}