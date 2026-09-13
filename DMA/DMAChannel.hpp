#pragma once
#include <cstdint>

namespace nova::ps1 {

class DMAChannel {
public:
    DMAChannel();
    ~DMAChannel() = default;

    void Reset();

    void SetBaseAddress(uint32_t address) { _baseAddress = address & 0x00FFFFFF; }
    void SetBlockControl(uint32_t value) { _blockControl = value; }
    void SetChannelControl(uint32_t value) { _channelControl = value; }

    uint32_t GetBaseAddress() const { return _baseAddress; }
    uint32_t GetBlockControl() const { return _blockControl; }
    uint32_t GetChannelControl() const { return _channelControl; }

    bool IsEnabled() const { return _enabled; }
    void SetEnabled(bool enabled) { _enabled = enabled; }

    bool IsTriggered() const { return _triggered; }
    void SetTriggered(bool triggered) { _triggered = triggered; }

    uint32_t GetSyncMode() const { return (_channelControl >> 9) & 0x3; }
    uint32_t GetDirection() const { return (_channelControl >> 1) & 0x1; }
    bool IsDecrement() const { return (_channelControl & 0x1) != 0; }

private:
    uint32_t _baseAddress;
    uint32_t _blockControl;
    uint32_t _channelControl;
    bool _enabled;
    bool _triggered;
};

}