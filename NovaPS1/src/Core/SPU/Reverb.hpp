#pragma once
#include <cstdint>
#include <array>

namespace nova::ps1 {

class Reverb {
public:
    Reverb();
    ~Reverb() = default;

    void Reset();

    int16_t Process(int16_t sample);

    void SetVolumeLeft(uint16_t value) { _volumeLeft = value; }
    void SetVolumeRight(uint16_t value) { _volumeRight = value; }

private:
    uint16_t _volumeLeft;
    uint16_t _volumeRight;

    std::array<int16_t, 8192> _bufferLeft;
    std::array<int16_t, 8192> _bufferRight;
    uint32_t _writePos;
};

}