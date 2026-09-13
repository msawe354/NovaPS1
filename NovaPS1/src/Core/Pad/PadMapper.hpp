#pragma once
#include "Pad.hpp"
#include <cstdint>
#include <unordered_map>

namespace nova::ps1 {

class PadMapper {
public:
    PadMapper();
    ~PadMapper() = default;

    void Reset();

    void MapKey(uint32_t keyCode, Pad::Button button);
    void UnmapKey(uint32_t keyCode);

    bool IsMapped(uint32_t keyCode) const;
    Pad::Button GetButton(uint32_t keyCode) const;

    void PressKey(Pad& pad, uint32_t port, uint32_t keyCode);
    void ReleaseKey(Pad& pad, uint32_t port, uint32_t keyCode);

private:
    std::unordered_map<uint32_t, Pad::Button> _keyMap;
};

}