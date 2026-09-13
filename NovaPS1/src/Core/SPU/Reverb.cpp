#include "Reverb.hpp"

namespace nova::ps1 {

Reverb::Reverb()
    : _volumeLeft(0), _volumeRight(0), _writePos(0) {
    Reset();
}

void Reverb::Reset() {
    _bufferLeft.fill(0);
    _bufferRight.fill(0);
    _writePos = 0;
}

int16_t Reverb::Process(int16_t sample) {
    int16_t delayed = _bufferLeft[_writePos];

    _bufferLeft[_writePos] = sample;
    _bufferRight[_writePos] = sample;
    _writePos = (_writePos + 1) % 8192;

    return delayed;
}

}