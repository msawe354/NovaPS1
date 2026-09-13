#include "CDROM.hpp"
#include "../CPU/Interrupts/Interrupts.hpp"
#include <fstream>
#include <cstring>
#include <iostream>

namespace nova::ps1 {

CDROM::CDROM()
    : _hasDisk(false),
      _hasBios(false),
      _status(0),
      _index(0),
      _command(0),
      _stat(0x02),
      _error(0),
      _irqEnable(0x1F),
      _irqFlag(0),
      _seekTarget(0),
      _currentSector(0),
      _readPosition(0),
      _dataFifoPosition(0),
      _parameterCount(0),
      _parameterReadPos(0),
      _responseCount(0),
      _responseReadPos(0),
      _dataBufferSize(0),
      _dataBufferPos(0),
      _cycleCounter(0),
      _pendingCycles(0),
      _state(State::Idle) {
    std::memset(_parameterFifo, 0, sizeof(_parameterFifo));
    std::memset(_responseFifo, 0, sizeof(_responseFifo));
    std::memset(_dataBuffer, 0, sizeof(_dataBuffer));
}

void CDROM::Reset() {
    _status = 0;
    _index = 0;
    _command = 0;
    _stat = 0x02;
    _error = 0;
    _irqEnable = 0x1F;
    _irqFlag = 0;
    _seekTarget = 0;
    _currentSector = 0;
    _readPosition = 0;
    _dataFifoPosition = 0;
    _parameterCount = 0;
    _parameterReadPos = 0;
    _responseCount = 0;
    _responseReadPos = 0;
    _dataBufferSize = 0;
    _dataBufferPos = 0;
    _cycleCounter = 0;
    _pendingCycles = 0;
    _state = State::Idle;
}

bool CDROM::LoadDisk(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "[CDROM] Cannot open disk: " << path << std::endl;
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    _diskData.resize(static_cast<size_t>(size));
    file.read(reinterpret_cast<char*>(_diskData.data()), size);
    file.close();

    _hasDisk = true;
    std::cout << "[CDROM] Disk loaded: " << path << " (" << size << " bytes)" << std::endl;
    return true;
}

bool CDROM::LoadBios(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "[CDROM] Cannot open BIOS: " << path << std::endl;
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    _biosData.resize(static_cast<size_t>(size));
    file.read(reinterpret_cast<char*>(_biosData.data()), size);
    file.close();

    _hasBios = true;
    std::cout << "[CDROM] BIOS loaded: " << path << " (" << size << " bytes)" << std::endl;
    return true;
}

uint32_t CDROM::GetSectorOffset(uint32_t sector) const {
    return sector * SECTOR_SIZE;
}

void CDROM::ReadSector(uint32_t sector) {
    if (!_hasDisk) return;

    uint32_t offset = GetSectorOffset(sector);
    if (offset + USER_SECTOR_SIZE > _diskData.size()) {
        _error = 0x80;
        return;
    }

    uint32_t dataOffset = offset + 24;

    std::memcpy(_dataBuffer, &_diskData[dataOffset], USER_SECTOR_SIZE);
    _dataBufferSize = USER_SECTOR_SIZE;
    _dataBufferPos = 0;
}

void CDROM::PushResponse(uint8_t value) {
    if (_responseCount < 16) {
        _responseFifo[_responseCount++] = value;
    }
}

void CDROM::PushResponseWord(uint16_t value) {
    PushResponse(static_cast<uint8_t>(value & 0xFF));
    PushResponse(static_cast<uint8_t>((value >> 8) & 0xFF));
}

void CDROM::PushResponseDword(uint32_t value) {
    PushResponse(static_cast<uint8_t>(value & 0xFF));
    PushResponse(static_cast<uint8_t>((value >> 8) & 0xFF));
    PushResponse(static_cast<uint8_t>((value >> 16) & 0xFF));
    PushResponse(static_cast<uint8_t>((value >> 24) & 0xFF));
}

void CDROM::ClearResponse() {
    _responseCount = 0;
    _responseReadPos = 0;
}

void CDROM::ClearParameters() {
    _parameterCount = 0;
    _parameterReadPos = 0;
}

void CDROM::TriggerIRQ(uint8_t flag) {
    _irqFlag = flag;

    if (_interrupts) {
        _interrupts->Request(Interrupts::IRQ_CDROM);
    }
}

void CDROM::UpdateStatus() {
    _stat = 0x02;

    if (_hasDisk) {
        _stat |= 0x10;
    }

    if (_state == State::Reading) {
        _stat |= 0x20;
    }
}

void CDROM::CommandGetStat() {
    ClearResponse();
    PushResponse(_stat);
    TriggerIRQ(0x03);
}

void CDROM::CommandGetID() {
    ClearResponse();
    PushResponse(_stat);
    PushResponse(0x00);
    PushResponse(0x20);
    PushResponse(0x00);
    PushResponse(0x00);
    TriggerIRQ(0x03);
}

void CDROM::CommandSetLoc() {
    if (_parameterCount < 3) return;
    _seekTarget = _parameterFifo[0] | (_parameterFifo[1] << 8) | (_parameterFifo[2] << 16);
    ClearResponse();
    PushResponse(_stat);
    TriggerIRQ(0x03);
}

void CDROM::CommandSeek() {
    _currentSector = _seekTarget;
    _state = State::Seeking;
    ClearResponse();
    PushResponse(_stat);
    TriggerIRQ(0x03);
}

void CDROM::CommandReadN() {
    _state = State::Reading;
    ClearResponse();
    PushResponse(_stat);
    TriggerIRQ(0x03);
}

void CDROM::CommandReadS() {
    _state = State::Reading;
    ClearResponse();
    PushResponse(_stat);
    TriggerIRQ(0x03);
}

void CDROM::CommandPause() {
    _state = State::Paused;
    ClearResponse();
    PushResponse(_stat);
    TriggerIRQ(0x03);
}

void CDROM::CommandInit() {
    _stat = 0x02;
    _state = State::Idle;
    _currentSector = 0;
    ClearResponse();
    PushResponse(_stat);
    TriggerIRQ(0x03);
}

void CDROM::CommandGetlocL() {
    ClearResponse();
    PushResponse(_stat);
    PushResponse(0x00);
    PushResponse(0x00);
    PushResponse(0x00);
    PushResponse(0x00);
    PushResponse(0x00);
    PushResponse(0x00);
    PushResponse(0x00);
    TriggerIRQ(0x03);
}

void CDROM::CommandGetlocP() {
    ClearResponse();
    PushResponse(_stat);
    PushResponse(0x00);
    PushResponse(0x00);
    PushResponse(0x00);
    PushResponse(0x00);
    TriggerIRQ(0x03);
}

void CDROM::CommandSetmode() {
    ClearResponse();
    PushResponse(_stat);
    TriggerIRQ(0x03);
}

void CDROM::CommandGetmode() {
    ClearResponse();
    PushResponse(_stat);
    PushResponse(0x00);
    TriggerIRQ(0x03);
}

void CDROM::CommandGetparam() {
    ClearResponse();
    PushResponse(_stat);
    PushResponse(0x00);
    PushResponse(0x00);
    TriggerIRQ(0x03);
}

void CDROM::CommandSetfilter() {
    ClearResponse();
    PushResponse(_stat);
    TriggerIRQ(0x03);
}

void CDROM::CommandSetSession() {
    ClearResponse();
    PushResponse(_stat);
    TriggerIRQ(0x03);
}

void CDROM::CommandTest() {
    ClearResponse();
    PushResponse(_stat);

    if (_parameterCount > 0 && _parameterFifo[0] == 0x20) {
        PushResponseDword(0x00000000);
    }

    TriggerIRQ(0x03);
}

void CDROM::ProcessCommand(uint8_t cmd) {
    switch (cmd) {
        case 0x01: CommandGetStat(); break;
        case 0x02: CommandGetID(); break;
        case 0x03: break;
        case 0x04: break;
        case 0x05: break;
        case 0x06: CommandReadN(); break;
        case 0x07: break;
        case 0x08: break;
        case 0x09: CommandPause(); break;
        case 0x0A: CommandInit(); break;
        case 0x0B: break;
        case 0x0C: break;
        case 0x0D: break;
        case 0x0E: CommandSetmode(); break;
        case 0x0F: CommandGetmode(); break;
        case 0x10: break;
        case 0x11: break;
        case 0x12: break;
        case 0x13: CommandGetlocL(); break;
        case 0x14: CommandGetlocP(); break;
        case 0x15: break;
        case 0x16: break;
        case 0x17: break;
        case 0x18: break;
        case 0x19: CommandTest(); break;
        case 0x1A: CommandGetID(); break;
        case 0x1B: CommandReadS(); break;
        case 0x1C: break;
        case 0x1D: break;
        case 0x1E: break;
        case 0x1F: break;
        case 0x20: break;
        case 0x21: break;
        case 0x22: break;
        case 0x23: break;
        case 0x24: break;
        case 0x25: break;
        case 0x26: break;
        case 0x27: break;
        case 0x28: break;
        case 0x29: break;
        case 0x2A: break;
        case 0x2B: break;
        case 0x2C: break;
        case 0x2D: break;
        case 0x2E: break;
        case 0x2F: break;
        default: break;
    }
}

uint32_t CDROM::ReadRegister(uint32_t address) {
    switch (address) {
        case 0x1F801800:
            return _index;

        case 0x1F801801:
            if (_responseReadPos < _responseCount) {
                return _responseFifo[_responseReadPos++];
            }
            return 0;

        case 0x1F801802:
            if (_dataBufferPos < _dataBufferSize) {
                return _dataBuffer[_dataBufferPos++];
            }
            return 0;

        case 0x1F801803:
            return _irqFlag;

        default:
            return 0;
    }
}

void CDROM::WriteRegister(uint32_t address, uint32_t value) {
    switch (address) {
        case 0x1F801800:
            _index = static_cast<uint8_t>(value & 0x3);
            break;

        case 0x1F801801:
            _command = static_cast<uint8_t>(value & 0xFF);
            ProcessCommand(_command);
            break;

        case 0x1F801802:
            if (_parameterCount < 16) {
                _parameterFifo[_parameterCount++] = static_cast<uint8_t>(value & 0xFF);
            }
            break;

        case 0x1F801803:
            if (_index == 0) {
                _irqEnable = static_cast<uint8_t>(value & 0x1F);
            } else if (_index == 1) {
                _irqEnable = static_cast<uint8_t>(value & 0x1F);
                _irqFlag &= ~static_cast<uint8_t>(value & 0x1F);
            }
            break;

        default:
            break;
    }
}

void CDROM::Step(uint32_t cycles) {
    _cycleCounter += cycles;

    if (_cycleCounter >= 1000) {
        _cycleCounter -= 1000;

        if (_state == State::Reading && _hasDisk) {
            ReadSector(_currentSector);
            _currentSector++;
        }
    }
}

}