#pragma once
#include "BIOS.hpp"
#include "../Bus/IBusDevice.hpp"

namespace nova::ps1 {

class BiosDevice : public IBusDevice {
public:
    BiosDevice();
    ~BiosDevice() override = default;

    void Reset();
    bool Load(const std::string& path);

    uint32_t Read(uint32_t offset, unsigned size) override;
    void Write(uint32_t offset, uint32_t value, unsigned size) override;

    BIOS& GetBIOS() { return _bios; }
    const BIOS& GetBIOS() const { return _bios; }

    bool IsLoaded() const { return _bios.IsLoaded(); }

private:
    BIOS _bios;
};

}