#pragma once
#include <cstdint>

namespace nova::ps1 {

class IBusDevice {
public:
    virtual ~IBusDevice() = default;

    virtual uint32_t Read(uint32_t offset, unsigned size) = 0;
    virtual void Write(uint32_t offset, uint32_t value, unsigned size) = 0;
};

}