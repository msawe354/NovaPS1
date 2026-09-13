#pragma once
#include <cstdint>
#include <array>

namespace nova::ps1 {

class Bus;

class DMA {
public:
    DMA();
    ~DMA() = default;

    void Reset();
    void SetBus(Bus* bus) { _bus = bus; }

    uint32_t ReadRegister(uint32_t address);
    void WriteRegister(uint32_t address, uint32_t value);

    void Step();

    static constexpr uint32_t CHANNEL_COUNT = 7;

    enum Channel : uint32_t {
        MDEC_IN = 0,
        MDEC_OUT = 1,
        GPU = 2,
        CDROM = 3,
        SPU = 4,
        PIO = 5,
        OTC = 6
    };

private:
    struct ChannelState {
        uint32_t baseAddress;
        uint32_t blockControl;
        uint32_t channelControl;
        bool enabled;
        bool triggered;
        uint32_t remainingWords;
        uint32_t currentAddress;

        ChannelState()
            : baseAddress(0), blockControl(0), channelControl(0),
              enabled(false), triggered(false),
              remainingWords(0), currentAddress(0) {}

        void Reset() {
            baseAddress = 0;
            blockControl = 0;
            channelControl = 0;
            enabled = false;
            triggered = false;
            remainingWords = 0;
            currentAddress = 0;
        }
    };

    std::array<ChannelState, CHANNEL_COUNT> _channels;

    Bus* _bus;

    uint32_t _controlRegister;
    uint32_t _interruptRegister;

    uint32_t _blockSize;
    uint32_t _blockCount;

    void TransferChannel(uint32_t channel);
    void TransferBlock(uint32_t channel);
    void TransferLinkedList(uint32_t channel);
    void UpdateInterrupt();

    uint32_t GetBlockSize() const { return _blockSize; }
    uint32_t GetBlockCount() const { return _blockCount; }
};

}