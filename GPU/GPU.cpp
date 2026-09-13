#include "GPU.hpp"
#include "GP0.hpp"
#include "GP1.hpp"

namespace nova::ps1 {

GPU::GPU()
    : _status(0x14802000),
      _displayMode(0),
      _displayEnabled(false),
      _displayVRAMStart(0),
      _horizontalStart(0x200),
      _horizontalEnd(0xC00),
      _verticalStart(0x10),
      _verticalEnd(0x100) {
    Reset();
}

void GPU::Reset() {
    _vram.Reset();
    _rasterizer.Reset();
    _commandBuffer.clear();
    _readBuffer.clear();
    _status = 0x14802000;
    _displayMode = 0;
    _displayEnabled = false;
    _displayVRAMStart = 0;
}

uint32_t GPU::Read(uint32_t offset, unsigned size) {
    switch (offset & 0x1F) {
        case 0x00:
            return ReadData();
        case 0x04:
            return ReadStatus();
        default:
            return 0;
    }
}

void GPU::Write(uint32_t offset, uint32_t value, unsigned size) {
    switch (offset & 0x1F) {
        case 0x00:
            WriteGP0(value);
            break;
        case 0x04:
            WriteGP1(value);
            break;
        default:
            break;
    }
}

void GPU::WriteGP0(uint32_t value) {
    PushCommand(value);
}

void GPU::WriteGP1(uint32_t value) {
    gpu::ExecuteGP1(*this, value);
}

void GPU::PushCommand(uint32_t value) {
    _commandBuffer.push_back(value);

    uint32_t firstWord = _commandBuffer[0];
    uint32_t command = (firstWord >> 24) & 0xFF;

    uint32_t requiredWords = 1;

    if ((command >= 0x20 && command <= 0x3F) || (command >= 0x60 && command <= 0x7F)) {
        if ((command & 0x04) != 0) requiredWords = 4;
        else if ((command & 0x08) != 0) requiredWords = 5;
        else if ((command & 0x10) != 0) requiredWords = 6;
        else requiredWords = 3;
    } else if (command >= 0x80 && command <= 0x9F) {
        requiredWords = 4;
    } else if (command >= 0xA0 && command <= 0xBF) {
        requiredWords = 4;
    } else if (command >= 0xC0 && command <= 0xDF) {
        requiredWords = 3;
    } else if (command >= 0xE0 && command <= 0xFF) {
        requiredWords = 1;
    } else if (command >= 0x40 && command <= 0x5F) {
        requiredWords = 3;
    }

    if (_commandBuffer.size() >= requiredWords) {
        FlushCommand();
    }
}

void GPU::FlushCommand() {
    if (_commandBuffer.empty()) return;

    gpu::ExecuteGP0(*this, _commandBuffer[0]);
    _commandBuffer.clear();
}

void GPU::ResetCommandBuffer() {
    _commandBuffer.clear();
    _readBuffer.clear();
}

void GPU::AcknowledgeIRQ() {
    _status &= ~0x01000000;
}

void GPU::SetDisplayEnable(bool enable) {
    _displayEnabled = enable;
    if (enable) _status |= 0x00800000;
    else _status &= ~0x00800000;
}

void GPU::SetDMADirection(uint32_t direction) {
    _status = (_status & ~0x00000600) | ((direction & 0x3) << 9);
}

void GPU::SetDisplayVRAMStart(uint32_t address) {
    _displayVRAMStart = address;
}

void GPU::SetHorizontalRange(uint32_t start, uint32_t end) {
    _horizontalStart = start;
    _horizontalEnd = end;
}

void GPU::SetVerticalRange(uint32_t start, uint32_t end) {
    _verticalStart = start;
    _verticalEnd = end;
}

void GPU::SetDisplayMode(uint32_t mode) {
    _displayMode = static_cast<uint8_t>(mode & 0xFF);
}

void GPU::SetGPUType(uint32_t type) {
    _status = (_status & ~0x00FF0000) | ((type & 0xFF) << 16);
}

uint16_t GPU::ReadStatus() const {
    return _status;
}

uint32_t GPU::ReadData() const {
    if (_readBuffer.empty()) return 0;
    return _readBuffer.front();
}

}