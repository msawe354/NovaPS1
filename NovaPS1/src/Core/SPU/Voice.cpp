#include "Voice.hpp"
#include "ADPCM.hpp"

namespace nova::ps1 {

Voice::Voice()
    : _startAddress(0), _currentAddress(0), _loopAddress(0),
      _sampleRate(44100), _counter(0),
      _volumeLeft(0), _volumeRight(0), _pitch(0),
      _adsrLow(0), _adsrHigh(0), _adsrCurrent(0), _adsrPhase(0),
      _enabled(false), _looping(false),
      _historyS1(0), _historyS2(0), _hasHistory(false) {
}

void Voice::Reset() {
    _startAddress = 0;
    _currentAddress = 0;
    _loopAddress = 0;
    _sampleRate = 44100;
    _counter = 0;
    _volumeLeft = 0;
    _volumeRight = 0;
    _pitch = 0;
    _adsrLow = 0;
    _adsrHigh = 0;
    _adsrCurrent = 0;
    _adsrPhase = 0;
    _enabled = false;
    _looping = false;
    _historyS1 = 0;
    _historyS2 = 0;
    _hasHistory = false;
}

void Voice::SetPitch(uint16_t pitch) {
    _pitch = pitch;
    _sampleRate = static_cast<uint32_t>(44100.0 * (pitch / 4096.0));
}

void Voice::UpdateADSR() {
    if (_adsrCurrent < 0x7FFF) {
        _adsrCurrent += 0x10;
        if (_adsrCurrent > 0x7FFF) _adsrCurrent = 0x7FFF;
    }
}

int16_t Voice::NextSample(uint8_t* ram) {
    if (!_enabled) return 0;

    if (_currentAddress + 16 > 0x80000) {
        _currentAddress = _loopAddress;
        return 0;
    }

    uint8_t shift = ram[_currentAddress + 1] & 0x0F;
    uint8_t filter = (ram[_currentAddress + 1] >> 4) & 0x0F;
    uint8_t flags = ram[_currentAddress + 1];

    int32_t s1 = _hasHistory ? _historyS1 : 0;
    int32_t s2 = _hasHistory ? _historyS2 : 0;

    int16_t result = ADPCM::DecodeSample(ram[_currentAddress], shift, filter, s1, s2);

    _historyS1 = result;
    _historyS2 = s1;
    _hasHistory = true;

    if (flags & 0x01) {
        _looping = true;
    }

    if (flags & 0x02) {
        _currentAddress = _loopAddress;
    } else {
        _currentAddress += 16;
    }

    if (flags & 0x04) {
        _enabled = false;
    }

    return result;
}

}