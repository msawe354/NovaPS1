#pragma once
#include "VRAM.hpp"
#include "Rasterizer.hpp"
#include "../Bus/IBusDevice.hpp"
#include <cstdint>
#include <vector>

namespace nova::ps1 {

class GPU : public IBusDevice {
public:
    GPU();
    ~GPU() override = default;

    void Reset();

    uint32_t Read(uint32_t offset, unsigned size) override;
    void Write(uint32_t offset, uint32_t value, unsigned size) override;

    void ResetCommandBuffer();
    void AcknowledgeIRQ();
    void SetDisplayEnable(bool enable);
    void SetDMADirection(uint32_t direction);
    void SetDisplayVRAMStart(uint32_t address);
    void SetHorizontalRange(uint32_t start, uint32_t end);
    void SetVerticalRange(uint32_t start, uint32_t end);
    void SetDisplayMode(uint32_t mode);
    void SetGPUType(uint32_t type);

    VRAM& GetVRAM() { return _vram; }
    const VRAM& GetVRAM() const { return _vram; }
    Rasterizer& GetRasterizer() { return _rasterizer; }

    uint16_t ReadStatus() const;
    uint32_t ReadData() const;
    void WriteGP0(uint32_t value);
    void WriteGP1(uint32_t value);

    void PushCommand(uint32_t value);
    void FlushCommand();

private:
    VRAM _vram;
    Rasterizer _rasterizer;

    std::vector<uint32_t> _commandBuffer;
    std::vector<uint32_t> _readBuffer;

    uint16_t _status;
    uint8_t _displayMode;
    bool _displayEnabled;
    uint32_t _displayVRAMStart;
    uint32_t _horizontalStart;
    uint32_t _horizontalEnd;
    uint32_t _verticalStart;
    uint32_t _verticalEnd;
};

}