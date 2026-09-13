#include "PadMapper.hpp"

namespace nova::ps1 {

PadMapper::PadMapper() {
    Reset();
}

void PadMapper::Reset() {
    _keyMap.clear();

    MapKey(0x26, Pad::Up);
    MapKey(0x28, Pad::Down);
    MapKey(0x25, Pad::Left);
    MapKey(0x27, Pad::Right);

    MapKey(0x58, Pad::Cross);
    MapKey(0x43, Pad::Circle);
    MapKey(0x56, Pad::Square);
    MapKey(0x54, Pad::Triangle);

    MapKey(0x51, Pad::L1);
    MapKey(0x57, Pad::R1);
    MapKey(0x41, Pad::L2);
    MapKey(0x53, Pad::R2);

    MapKey(0x0D, Pad::Start);
    MapKey(0x10, Pad::Select);

    MapKey(0x4C, Pad::L3);
    MapKey(0x4E, Pad::R3);
}

void PadMapper::MapKey(uint32_t keyCode, Pad::Button button) {
    _keyMap[keyCode] = button;
}

void PadMapper::UnmapKey(uint32_t keyCode) {
    _keyMap.erase(keyCode);
}

bool PadMapper::IsMapped(uint32_t keyCode) const {
    return _keyMap.find(keyCode) != _keyMap.end();
}

Pad::Button PadMapper::GetButton(uint32_t keyCode) const {
    auto it = _keyMap.find(keyCode);
    if (it == _keyMap.end()) return static_cast<Pad::Button>(0);
    return it->second;
}

void PadMapper::PressKey(Pad& pad, uint32_t port, uint32_t keyCode) {
    if (!IsMapped(keyCode)) return;
    pad.PressButton(port, GetButton(keyCode));
}

void PadMapper::ReleaseKey(Pad& pad, uint32_t port, uint32_t keyCode) {
    if (!IsMapped(keyCode)) return;
    pad.ReleaseButton(port, GetButton(keyCode));
}

}