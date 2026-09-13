#pragma once
#include <cstdint>
#include <string>

namespace nova::ps1 {

class Bus;
class MIPS;

class BIOSHLE {
public:
    BIOSHLE();
    ~BIOSHLE() = default;

    void Reset();
    void SetBus(Bus* bus) { _bus = bus; }

    void HandleA0(MIPS& cpu, uint32_t function);
    void HandleB0(MIPS& cpu, uint32_t function);
    void HandleC0(MIPS& cpu, uint32_t function);

private:
    Bus* _bus;

    void A0_FlushCache(MIPS& cpu);
    void A0_PutChar(MIPS& cpu);
    void A0_PutString(MIPS& cpu);
    void A0_LoadExe(MIPS& cpu);
    void A0_Exit(MIPS& cpu);
    void A0_OpenEvent(MIPS& cpu);
    void A0_CloseEvent(MIPS& cpu);
    void A0_WaitEvent(MIPS& cpu);
    void A0_TestEvent(MIPS& cpu);
    void A0_EnableEvent(MIPS& cpu);
    void A0_DisableEvent(MIPS& cpu);
    void A0_DeliverEvent(MIPS& cpu);
    void A0_GetLastError(MIPS& cpu);

    std::string ReadString(uint32_t address, uint32_t maxLength);
    void PrintString(const std::string& str);
};

}