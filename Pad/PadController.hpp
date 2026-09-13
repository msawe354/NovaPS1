#pragma once
#include "Pad.hpp"
#include "PadMapper.hpp"
#include <cstdint>

namespace nova::ps1 {

class PadController {
public:
    PadController();
    ~PadController() = default;

    void Reset();
    void SetPad(Pad* pad) { _pad = pad; }

    void OnKeyDown(uint32_t keyCode);
    void OnKeyUp(uint32_t keyCode);

    void SetActivePort(uint32_t port) { _activePort = port; }
    uint32_t GetActivePort() const { return _activePort; }

private:
    Pad* _pad;
    PadMapper _mapper;
    uint32_t _activePort;
};

}