#include "SPU.hpp"
#include <cstring>

namespace nova::ps1 {

SPU::SPU()
    : _mainVolumeLeft(0x3FFF),
      _mainVolumeRight(0x3FFF),
      _reverbVolumeLeft(0),
      _reverbVolumeRight(0),
      _controlRegister(0),
      _statusRegister(0),
      _transferAddress(0),
      _transferControl(0),
      _bufferWritePos(0),
      _bufferReadPos(0),
      _sampleCount(0),
      _cycleCounter(0) {
    Reset();
}

void SPU::Reset() {
    for (auto& voice : _voices) {
        voice.Reset();
    }

    _ram.fill(0);
    _mainVolumeLeft = 0x3FFF;
    _mainVolumeRight = 0x3FFF;
    _reverbVolumeLeft = 0;
    _reverbVolumeRight = 0;
    _controlRegister = 0;
    _statusRegister = 0;
    _transferAddress = 0;
    _transferControl = 0;

    _bufferLeft.fill(0);
    _bufferRight.fill(0);
    _bufferWritePos = 0;
    _bufferReadPos = 0;
    _sampleCount = 0;
    _cycleCounter = 0;
}

uint16_t SPU::ReadRegister(uint32_t address) {
    if (address >= 0x1F801C00 && address <= 0x1F801D7F) {
        uint32_t voice = (address - 0x1F801C00) / 0x10;
        uint32_t reg = ((address - 0x1F801C00) % 0x10) / 2;
        return ReadVoiceRegister(voice, reg);
    }

    switch (address) {
        case 0x1F801D80: return _mainVolumeLeft;
        case 0x1F801D82: return _mainVolumeRight;
        case 0x1F801D84: return _reverbVolumeLeft;
        case 0x1F801D86: return _reverbVolumeRight;
        case 0x1F801DAA: return _controlRegister;
        case 0x1F801DAE: return _statusRegister;
        case 0x1F801DA6: return _transferAddress;
        default: return 0;
    }
}

void SPU::WriteRegister(uint32_t address, uint16_t value) {
    if (address >= 0x1F801C00 && address <= 0x1F801D7F) {
        uint32_t voice = (address - 0x1F801C00) / 0x10;
        uint32_t reg = ((address - 0x1F801C00) % 0x10) / 2;
        WriteVoiceRegister(voice, reg, value);
        return;
    }

    switch (address) {
        case 0x1F801D80:
            _mainVolumeLeft = value;
            break;
        case 0x1F801D82:
            _mainVolumeRight = value;
            break;
        case 0x1F801D84:
            _reverbVolumeLeft = value;
            break;
        case 0x1F801D86:
            _reverbVolumeRight = value;
            break;
        case 0x1F801D88:
            for (uint32_t i = 0; i < 16; i++) {
                if (value & (1u << i)) _voices[i].enabled = true;
            }
            break;
        case 0x1F801D8A:
            for (uint32_t i = 16; i < 24; i++) {
                if (value & (1u << (i - 16))) _voices[i].enabled = true;
            }
            break;
        case 0x1F801D8C:
            for (uint32_t i = 0; i < 16; i++) {
                if (value & (1u << i)) _voices[i].enabled = false;
            }
            break;
        case 0x1F801D8E:
            for (uint32_t i = 16; i < 24; i++) {
                if (value & (1u << (i - 16))) _voices[i].enabled = false;
            }
            break;
        case 0x1F801DAA:
            _controlRegister = value;
            if (value & 0x8000) {
                _statusRegister |= 0x0080;
            }
            break;
        case 0x1F801DA6:
            _transferAddress = value;
            break;
        case 0x1F801DA8:
            break;
        case 0x1F801DAC:
            _transferControl = value;
            break;
        default:
            break;
    }
}

uint16_t SPU::ReadVoiceRegister(uint32_t voice, uint32_t reg) {
    if (voice >= VOICE_COUNT) return 0;
    const Voice& v = _voices[voice];

    switch (reg) {
        case 0: return static_cast<uint16_t>(v.volumeLeft);
        case 1: return static_cast<uint16_t>(v.volumeRight);
        case 2: return static_cast<uint16_t>(v.pitch);
        case 3: return static_cast<uint16_t>(v.startAddress);
        case 4: return static_cast<uint16_t>(v.adsrLow);
        case 5: return static_cast<uint16_t>(v.adsrHigh);
        case 6: return static_cast<uint16_t>(v.adsrCurrent);
        case 7: return static_cast<uint16_t>(v.repeatAddress);
        default: return 0;
    }
}

void SPU::WriteVoiceRegister(uint32_t voice, uint32_t reg, uint16_t value) {
    if (voice >= VOICE_COUNT) return;
    Voice& v = _voices[voice];

    switch (reg) {
        case 0:
            v.volumeLeft = value;
            break;
        case 1:
            v.volumeRight = value;
            break;
        case 2:
            v.pitch = value;
            v.sampleRate = static_cast<uint32_t>(44100.0 * (value / 4096.0));
            break;
        case 3:
            v.startAddress = value * 8;
            v.currentAddress = v.startAddress;
            break;
        case 4:
            v.adsrLow = value;
            break;
        case 5:
            v.adsrHigh = value;
            v.adsrPhase = 0;
            v.adsrCurrent = 0;
            break;
        case 6:
            v.adsrCurrent = value;
            break;
        case 7:
            v.repeatAddress = value * 8;
            break;
        default:
            break;
    }
}

int16_t SPU::DecodeADPCM(uint32_t voiceIndex) {
    if (voiceIndex >= VOICE_COUNT) return 0;
    Voice& v = _voices[voiceIndex];

    if (v.currentAddress + 16 > RAM_SIZE) {
        v.currentAddress = v.repeatAddress;
        return 0;
    }

    uint8_t shift = _ram[v.currentAddress + 1] & 0x0F;
    uint8_t filter = (_ram[v.currentAddress + 1] >> 4) & 0x0F;
    uint8_t flags = _ram[v.currentAddress + 1];

    static const int16_t filterTable[5][2] = {
        {0, 0},
        {60, 0},
        {115, -52},
        {98, -55},
        {122, -60}
    };

    int32_t s1 = 0;
    int32_t s2 = 0;

    if (filter > 0 && filter < 5) {
        s1 = v.counter & 0xFFFF;
        s2 = (v.counter >> 16) & 0xFFFF;
    }

    int32_t sample = 0;
    if (filter < 5) {
        sample = s1 * filterTable[filter][0] + s2 * filterTable[filter][1];
        sample >>= 6;
    }

    int16_t result = static_cast<int16_t>(sample);

    if (flags & 0x01) {
        v.looping = true;
    }

    if (flags & 0x02) {
        v.currentAddress = v.repeatAddress;
    } else {
        v.currentAddress += 16;
    }

    if (flags & 0x04) {
    }

    return result;
}

void SPU::UpdateADSR(Voice& voice) {
    uint16_t adsrLow = voice.adsrLow;
    uint16_t adsrHigh = voice.adsrHigh;

    uint8_t attackMode = adsrLow & 0x0F;
    uint8_t decayShift = (adsrLow >> 4) & 0x0F;
    uint8_t sustainLevel = (adsrLow >> 8) & 0x0F;
    uint8_t sustainMode = (adsrHigh >> 4) & 0x0F;
    uint8_t releaseMode = adsrHigh & 0x0F;

    (void)attackMode;
    (void)decayShift;
    (void)sustainLevel;
    (void)sustainMode;
    (void)releaseMode;

    if (voice.adsrCurrent < 0x7FFF) {
        voice.adsrCurrent += 0x10;
    }
}

void SPU::MixSamples() {
    int32_t mixLeft = 0;
    int32_t mixRight = 0;

    for (uint32_t i = 0; i < VOICE_COUNT; i++) {
        Voice& v = _voices[i];
        if (!v.enabled) continue;

        int16_t sample = DecodeADPCM(i);

        UpdateADSR(v);

        int32_t adsr = v.adsrCurrent >> 15;
        int32_t leftVol = static_cast<int16_t>(v.volumeLeft);
        int32_t rightVol = static_cast<int16_t>(v.volumeRight);

        mixLeft += (sample * leftVol * adsr) >> 15;
        mixRight += (sample * rightVol * adsr) >> 15;
    }

    int32_t mainLeft = static_cast<int16_t>(_mainVolumeLeft);
    int32_t mainRight = static_cast<int16_t>(_mainVolumeRight);

    mixLeft = (mixLeft * mainLeft) >> 15;
    mixRight = (mixRight * mainRight) >> 15;

    if (mixLeft > 32767) mixLeft = 32767;
    if (mixLeft < -32768) mixLeft = -32768;
    if (mixRight > 32767) mixRight = 32767;
    if (mixRight < -32768) mixRight = -32768;

    PushSample(static_cast<int16_t>(mixLeft), static_cast<int16_t>(mixRight));
}

void SPU::Step(uint32_t cycles) {
    _cycleCounter += cycles;

    if (_cycleCounter >= 768) {
        _cycleCounter -= 768;
        MixSamples();
    }
}

void SPU::PushSample(int16_t left, int16_t right) {
    if (_sampleCount >= 4096) return;

    _bufferLeft[_bufferWritePos] = left;
    _bufferRight[_bufferWritePos] = right;
    _bufferWritePos = (_bufferWritePos + 1) % 4096;
    _sampleCount++;
}

int16_t SPU::PopLeft() {
    if (_sampleCount == 0) return 0;
    int16_t value = _bufferLeft[_bufferReadPos];
    _bufferReadPos = (_bufferReadPos + 1) % 4096;
    _sampleCount--;
    return value;
}

int16_t SPU::PopRight() {
    if (_sampleCount == 0) return 0;
    return _bufferRight[_bufferReadPos];
}

}