#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <string>

namespace nova::ps1 {

class Interrupts;

class CDROM {
public:
    CDROM();
    ~CDROM() = default;

    void Reset();
    void SetInterrupts(Interrupts* interrupts) { _interrupts = interrupts; }

    bool LoadDisk(const std::string& path);
    bool LoadBios(const std::string& path);

    void Step(uint32_t cycles);

    uint32_t ReadRegister(uint32_t address);
    void WriteRegister(uint32_t address, uint32_t value);

    static constexpr uint32_t SECTOR_SIZE = 2352;
    static constexpr uint32_t USER_SECTOR_SIZE = 2048;

    bool HasDisk() const { return _hasDisk; }
    uint32_t GetDiskSize() const { return static_cast<uint32_t>(_diskData.size()); }

private:
    std::vector<uint8_t> _diskData;
    std::vector<uint8_t> _biosData;

    bool _hasDisk;
    bool _hasBios;

    uint8_t _status;
    uint8_t _index;
    uint8_t _command;
    uint8_t _stat;
    uint8_t _error;

    uint8_t _irqEnable;
    uint8_t _irqFlag;

    uint32_t _seekTarget;
    uint32_t _currentSector;
    uint32_t _readPosition;
    uint32_t _dataFifoPosition;

    uint8_t _parameterFifo[16];
    uint8_t _parameterCount;
    uint8_t _parameterReadPos;

    uint8_t _responseFifo[16];
    uint8_t _responseCount;
    uint8_t _responseReadPos;

    uint8_t _dataBuffer[USER_SECTOR_SIZE];
    uint32_t _dataBufferSize;
    uint32_t _dataBufferPos;

    uint32_t _cycleCounter;
    uint32_t _pendingCycles;

    enum class State {
        Idle,
        Seeking,
        Reading,
        Playing,
        Paused
    };

    State _state;

    void ExecuteCommand(uint8_t cmd, const uint8_t* params, uint8_t paramCount);
    void ProcessCommand(uint8_t cmd);
    void PushResponse(uint8_t value);
    void PushResponseWord(uint16_t value);
    void PushResponseDword(uint32_t value);
    void ClearResponse();
    void ClearParameters();
    void TriggerIRQ(uint8_t flag);
    void UpdateStatus();

    void CommandGetStat();
    void CommandGetID();
    void CommandSetLoc();
    void CommandSeek();
    void CommandReadN();
    void CommandReadS();
    void CommandPause();
    void CommandInit();
    void CommandGetlocL();
    void CommandGetlocP();
    void CommandSetmode();
    void CommandGetmode();
    void CommandGetparam();
    void CommandSetfilter();
    void CommandSetSession();
    void CommandTest();

    void ReadSector(uint32_t sector);
    uint32_t GetSectorOffset(uint32_t sector) const;
};

}