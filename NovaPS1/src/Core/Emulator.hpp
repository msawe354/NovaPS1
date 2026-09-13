#pragma once
#include "Memory/Bus.hpp"
#include "CPU/MIPS.hpp"
#include "GPU/GPU.hpp"
#include "SPU/SPU.hpp"
#include "DMA/DMA.hpp"
#include "Timers/Timers.hpp"
#include "CDROM/CDROM.hpp"
#include "BIOS/BIOS.hpp"
#include "Pad/Pad.hpp"
#include <cstdint>
#include <string>

namespace nova::ps1 {

class Emulator {
public:
    Emulator();
    ~Emulator();

    bool Initialize();
    bool LoadBios(const std::string& path);
    bool LoadDisk(const std::string& path);
    bool LoadExe(const std::string& path);

    void Reset();
    void Run();
    void Stop();

    void Step();

    uint32_t GetFramebufferWidth() const { return 1024; }
    uint32_t GetFramebufferHeight() const { return 512; }
    const uint8_t* GetFramebuffer() const;

    uint64_t GetCycleCount() const { return _cycleCount; }
    bool IsRunning() const { return _running; }

    Bus& GetBus() { return _bus; }
    MIPS& GetCPU() { return _cpu; }
    GPU& GetGPU() { return _gpu; }
    SPU& GetSPU() { return _spu; }
    Pad& GetPad() { return _pad; }

    void OnKeyDown(uint32_t keyCode);
    void OnKeyUp(uint32_t keyCode);

private:
    Bus _bus;
    MIPS _cpu;
    GPU _gpu;
    SPU _spu;
    DMA _dma;
    Timers _timers;
    CDROM _cdrom;
    BIOS _bios;
    Pad _pad;

    uint64_t _cycleCount;
    bool _running;

    void InitializeDevices();
    void SyncComponents();
    void HandleFramebuffer();
};

}