#pragma once
#include <cstdint>
#include <array>

namespace nova::ps1 {

class SPU {
public:
    SPU();
    ~SPU() = default;

    void Reset();
    void Step(uint32_t cycles);

    uint16_t ReadRegister(uint32_t address);
    void WriteRegister(uint32_t address, uint16_t value);

    void PushSample(int16_t left, int16_t right);
    bool HasSamples() const { return _sampleCount > 0; }
    int16_t PopLeft();
    int16_t PopRight();

    static constexpr uint32_t VOICE_COUNT = 24;
    static constexpr uint32_t RAM_SIZE = 512 * 1024;
    static constexpr uint32_t SAMPLE_RATE = 44100;

private:
    struct Voice {
        uint32_t startAddress;
        uint32_t currentAddress;
        uint32_t loopAddress;
        uint32_t sampleRate;
        uint32_t counter;
        uint16_t volumeLeft;
        uint16_t volumeRight;
        uint16_t pitch;
        uint16_t adsrLow;
        uint16_t adsrHigh;
        uint16_t adsrCurrent;
        uint16_t repeatAddress;
        bool enabled;
        bool looping;
        uint8_t adsrPhase;

        Voice()
            : startAddress(0), currentAddress(0), loopAddress(0),
              sampleRate(44100), counter(0),
              volumeLeft(0), volumeRight(0), pitch(0),
              adsrLow(0), adsrHigh(0), adsrCurrent(0), repeatAddress(0),
              enabled(false), looping(false), adsrPhase(0) {}

        void Reset() {
            startAddress = 0;
            currentAddress = 0;
            loopAddress = 0;
            sampleRate = 44100;
            counter = 0;
            volumeLeft = 0;
            volumeRight = 0;
            pitch = 0;
            adsrLow = 0;
            adsrHigh = 0;
            adsrCurrent = 0;
            repeatAddress = 0;
            enabled = false;
            looping = false;
            adsrPhase = 0;
        }
    };

    std::array<Voice, VOICE_COUNT> _voices;
    std::array<uint8_t, RAM_SIZE> _ram;

    uint16_t _mainVolumeLeft;
    uint16_t _mainVolumeRight;
    uint16_t _reverbVolumeLeft;
    uint16_t _reverbVolumeRight;
    uint16_t _controlRegister;
    uint16_t _statusRegister;
    uint16_t _transferAddress;
    uint16_t _transferControl;

    std::array<int16_t, 4096> _bufferLeft;
    std::array<int16_t, 4096> _bufferRight;
    uint32_t _bufferWritePos;
    uint32_t _bufferReadPos;
    uint32_t _sampleCount;

    uint32_t _cycleCounter;

    int16_t DecodeADPCM(uint32_t voiceIndex);
    void UpdateADSR(Voice& voice);
    void MixSamples();

    uint16_t ReadVoiceRegister(uint32_t voice, uint32_t reg);
    void WriteVoiceRegister(uint32_t voice, uint32_t reg, uint16_t value);
};

}