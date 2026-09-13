#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace nova::ps1 {

class Bus;
class MIPS;

class BIOS {
public:
    BIOS();
    ~BIOS() = default;

    void Reset();

    bool Load(const std::string& path);
    bool IsLoaded() const { return _loaded; }

    void AttachBus(Bus* bus) { _bus = bus; }
    void AttachCPU(MIPS* cpu) { _cpu = cpu; }

    void HandleSyscall(MIPS& cpu, uint32_t code);

    uint8_t Read8(uint32_t offset) const;
    uint16_t Read16(uint32_t offset) const;
    uint32_t Read32(uint32_t offset) const;

    uint32_t GetEntryPoint() const { return 0xBFC00000; }

    static constexpr uint32_t BIOS_SIZE = 0x80000;

private:
    std::vector<uint8_t> _data;
    Bus* _bus;
    MIPS* _cpu;
    bool _loaded;

    void SyscallA(MIPS& cpu);
    void SyscallB(MIPS& cpu);
    void SyscallC(MIPS& cpu);

    void HLE_PutChar(MIPS& cpu);
    void HLE_PutString(MIPS& cpu);
    void HLE_Exit(MIPS& cpu);
    void HLE_LoadExe(MIPS& cpu);
    void HLE_FlushCache(MIPS& cpu);
};

}