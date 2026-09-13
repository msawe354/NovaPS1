#pragma once
#include <cstdint>

namespace nova::ps1 {

class Voice {
public:
    Voice();
    ~Voice() = default;

    void Reset();

    void SetEnabled(bool enabled) { _enabled = enabled; }
    bool IsEnabled() const { return _enabled; }

    void SetStartAddress(uint32_t address) { _startAddress = address * 8; _currentAddress = _startAddress; }
    void SetLoopAddress(uint32_t address) { _loopAddress = address * 8; }
    void SetPitch(uint16_t pitch);

    uint32_t GetCurrentAddress() const { return _currentAddress; }
    void SetCurrentAddress(uint32_t address) { _currentAddress = address; }

    uint16_t GetVolumeLeft() const { return _volumeLeft; }
    uint16_t GetVolumeRight() const { return _volumeRight; }

    void SetVolumeLeft(uint16_t value) { _volumeLeft = value; }
    void SetVolumeRight(uint16_t value) { _volumeRight = value; }

    uint16_t GetADSRCurrent() const { return _adsrCurrent; }
    void SetADSRCurrent(uint16_t value) { _adsrCurrent = value; }

    void UpdateADSR();
    int16_t NextSample(uint8_t* ram);

private:
    uint32_t _startAddress;
    uint32_t _currentAddress;
    uint32_t _loopAddress;
    uint32_t _sampleRate;
    uint32_t _counter;

    uint16_t _volumeLeft;
    uint16_t _volumeRight;
    uint16_t _pitch;
    uint16_t _adsrLow;
    uint16_t _adsrHigh;
    uint16_t _adsrCurrent;
    uint8_t _adsrPhase;

    bool _enabled;
    bool _looping;

    int32_t _historyS1;
    int32_t _historyS2;
    bool _hasHistory;
};

}