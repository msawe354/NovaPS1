#include "DMA.hpp"
#include "../Memory/Bus.hpp"

namespace nova::ps1 {

DMA::DMA()
    : _bus(nullptr),
      _controlRegister(0),
      _interruptRegister(0),
      _blockSize(0),
      _blockCount(0) {
    Reset();
}

void DMA::Reset() {
    for (auto& ch : _channels) {
        ch.Reset();
    }
    _controlRegister = 0;
    _interruptRegister = 0;
    _blockSize = 0;
    _blockCount = 0;
}

uint32_t DMA::ReadRegister(uint32_t address) {
    if (address >= 0x1F801080 && address <= 0x1F8010EF) {
        uint32_t channel = (address - 0x1F801080) / 0x10;
        uint32_t reg = ((address - 0x1F801080) % 0x10) / 4;

        if (channel >= CHANNEL_COUNT) return 0;

        switch (reg) {
            case 0: return _channels[channel].baseAddress;
            case 1: return _channels[channel].blockControl;
            case 2: return _channels[channel].channelControl;
            default: return 0;
        }
    }

    switch (address) {
        case 0x1F8010F0: return _controlRegister;
        case 0x1F8010F4: return _interruptRegister;
        default: return 0;
    }
}

void DMA::WriteRegister(uint32_t address, uint32_t value) {
    if (address >= 0x1F801080 && address <= 0x1F8010EF) {
        uint32_t channel = (address - 0x1F801080) / 0x10;
        uint32_t reg = ((address - 0x1F801080) % 0x10) / 4;

        if (channel >= CHANNEL_COUNT) return;

        switch (reg) {
            case 0:
                _channels[channel].baseAddress = value & 0x00FFFFFF;
                break;
            case 1:
                _channels[channel].blockControl = value;
                _blockSize = value & 0xFFFF;
                _blockCount = (value >> 16) & 0xFFFF;
                if (_blockSize == 0) _blockSize = 0x10000;
                if (_blockCount == 0) _blockCount = 0x10000;
                break;
            case 2:
                _channels[channel].channelControl = value;
                _channels[channel].enabled = (value & 0x01000000) != 0;
                if (_channels[channel].enabled) {
                    _channels[channel].triggered = true;
                }
                break;
            default:
                break;
        }
        return;
    }

    switch (address) {
        case 0x1F8010F0:
            _controlRegister = value;
            break;
        case 0x1F8010F4:
            _interruptRegister &= ~(value & 0x7F000000);
            break;
        case 0x1F8010F6:
            _interruptRegister &= ~(value & 0x7F000000);
            break;
        default:
            break;
    }
}

void DMA::TransferChannel(uint32_t channel) {
    if (channel >= CHANNEL_COUNT) return;

    ChannelState& ch = _channels[channel];
    if (!ch.triggered) return;

    uint32_t syncMode = (ch.channelControl >> 9) & 0x3;

    switch (syncMode) {
        case 0:
            TransferBlock(channel);
            break;
        case 1:
            TransferBlock(channel);
            break;
        case 2:
            TransferLinkedList(channel);
            break;
        default:
            break;
    }

    ch.triggered = false;
    ch.enabled = false;
    UpdateInterrupt();
}

void DMA::TransferBlock(uint32_t channel) {
    if (!_bus) return;

    ChannelState& ch = _channels[channel];

    uint32_t direction = (ch.channelControl >> 1) & 0x1;
    uint32_t step = (ch.channelControl & 0x1) ? 0xFFFFFFFC : 0x4;

    uint32_t wordCount = _blockSize * _blockCount;

    uint32_t address = ch.baseAddress;

    for (uint32_t i = 0; i < wordCount; i++) {
        uint32_t value = _bus->Read32(address);

        if (channel == GPU) {
            uint32_t gpuAddress = 0x1F801810;
            _bus->Write32(gpuAddress, value);
        } else if (channel == SPU) {
            uint32_t spuAddress = 0x1F801DA8;
            _bus->Write32(spuAddress, value);
        }

        if (direction == 0) {
            address = (address + step) & 0x00FFFFFF;
        } else {
            address = (address - step) & 0x00FFFFFF;
        }
    }
}

void DMA::TransferLinkedList(uint32_t channel) {
    if (!_bus) return;

    ChannelState& ch = _channels[channel];
    uint32_t address = ch.baseAddress & 0x1FFFFC;

    while (true) {
        uint32_t header = _bus->Read32(address);

        uint32_t wordCount = header >> 24;
        uint32_t nextAddress = header & 0x00FFFFFF;

        uint32_t dataAddress = address + 4;

        for (uint32_t i = 0; i < wordCount; i++) {
            uint32_t value = _bus->Read32(dataAddress);

            if (channel == GPU) {
                uint32_t gpuAddress = 0x1F801810;
                _bus->Write32(gpuAddress, value);
            }

            dataAddress = (dataAddress + 4) & 0x1FFFFC;
        }

        if (nextAddress & 0x00800000) {
            break;
        }

        address = nextAddress & 0x1FFFFC;
    }
}

void DMA::UpdateInterrupt() {
    _interruptRegister |= 0x80000000;
}

void DMA::Step() {
    for (uint32_t i = 0; i < CHANNEL_COUNT; i++) {
        if (_channels[i].triggered) {
            TransferChannel(i);
        }
    }
}

}