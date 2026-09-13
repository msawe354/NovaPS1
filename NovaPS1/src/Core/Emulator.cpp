#include "Emulator.hpp"
#include "Bus/Bus.hpp"
#include <iostream>
#include <fstream>

namespace nova::ps1 {

Emulator::Emulator()
    : _bus(),
      _cpu(_bus),
      _gpu(),
      _spu(),
      _dma(),
      _timers(),
      _cdrom(),
      _bios(),
      _pad(),
      _cycleCount(0),
      _running(false) {
}

Emulator::~Emulator() {
    Stop();
}

bool Emulator::Initialize() {
    InitializeDevices();
    return true;
}

void Emulator::InitializeDevices() {
    _dma.SetBus(&_bus);
    _dma.Reset();

    _timers.Reset();
    _timers.SetInterrupts(&_cpu.GetInterrupts());

    _cdrom.Reset();
    _cdrom.SetInterrupts(&_cpu.GetInterrupts());

    _pad.Reset();
    _pad.SetInterrupts(&_cpu.GetInterrupts());

    _bios.AttachBus(&_bus);
    _bios.AttachCPU(&_cpu);

    _bus.AttachDevice(0x1F801000, 0x2000, &_gpu);
    _bus.AttachDevice(0x1F801080, 0x80, &_dma);
    _bus.AttachDevice(0x1F801100, 0x30, &_timers);
    _bus.AttachDevice(0x1F801800, 0x04, &_cdrom);
    _bus.AttachDevice(0x1F801C00, 0x400, &_spu);
    _bus.AttachDevice(0x1F801040, 0x10, &_pad);
}

bool Emulator::LoadBios(const std::string& path) {
    if (!_bios.Load(path)) {
        std::cerr << "[Emulator] Failed to load BIOS" << std::endl;
        return false;
    }

    std::vector<uint8_t> biosData;
    for (uint32_t i = 0; i < BIOS::BIOS_SIZE; i++) {
        biosData.push_back(_bios.Read8(i));
    }

    _bus.LoadBios(biosData);
    return true;
}

bool Emulator::LoadDisk(const std::string& path) {
    return _cdrom.LoadDisk(path);
}

bool Emulator::LoadExe(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "[Emulator] Cannot open EXE: " << path << std::endl;
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> data(static_cast<size_t>(size));
    file.read(reinterpret_cast<char*>(data.data()), size);
    file.close();

    if (data.size() < 0x800) {
        std::cerr << "[Emulator] EXE too small" << std::endl;
        return false;
    }

    uint32_t magic = static_cast<uint32_t>(data[0])
                   | (static_cast<uint32_t>(data[1]) << 8)
                   | (static_cast<uint32_t>(data[2]) << 16)
                   | (static_cast<uint32_t>(data[3]) << 24);

    if (magic != 0x45584550) {
        std::cerr << "[Emulator] Invalid EXE magic: 0x" << std::hex << magic << std::dec << std::endl;
        return false;
    }

    uint32_t pc = static_cast<uint32_t>(data[0x10])
                | (static_cast<uint32_t>(data[0x11]) << 8)
                | (static_cast<uint32_t>(data[0x12]) << 16)
                | (static_cast<uint32_t>(data[0x13]) << 24);

    uint32_t loadAddr = static_cast<uint32_t>(data[0x18])
                      | (static_cast<uint32_t>(data[0x19]) << 8)
                      | (static_cast<uint32_t>(data[0x1A]) << 16)
                      | (static_cast<uint32_t>(data[0x1B]) << 24);

    uint32_t loadSize = static_cast<uint32_t>(data[0x1C])
                      | (static_cast<uint32_t>(data[0x1D]) << 8)
                      | (static_cast<uint32_t>(data[0x1E]) << 16)
                      | (static_cast<uint32_t>(data[0x1F]) << 24);

    for (uint32_t i = 0; i < loadSize && (0x800 + i) < data.size(); i++) {
        _bus.Write8(loadAddr + i, data[0x800 + i]);
    }

    _cpu.GetRegisters().SetPC(pc);
    _cpu.GetRegisters().SetGPR(28, loadAddr);

    std::cout << "[Emulator] EXE loaded: PC=0x" << std::hex << pc
              << " Load=0x" << loadAddr
              << " Size=0x" << loadSize << std::dec << std::endl;

    return true;
}

void Emulator::Reset() {
    _cpu.Reset();
    _gpu.Reset();
    _spu.Reset();
    _dma.Reset();
    _timers.Reset();
    _cdrom.Reset();
    _pad.Reset();

    _cycleCount = 0;
    _running = false;
}

void Emulator::Stop() {
    _running = false;
}

void Emulator::SyncComponents() {
    _spu.Step(1);
    _dma.Step();
    _timers.Step(1);
    _cdrom.Step(1);
    _pad.Step(1);
}

void Emulator::Step() {
    _cpu.Step();
    SyncComponents();
    _cycleCount++;
}

void Emulator::Run() {
    _running = true;

    while (_running) {
        Step();

        if (_cycleCount % 1000000 == 0) {
            HandleFramebuffer();
        }
    }
}

void Emulator::HandleFramebuffer() {
}

const uint8_t* Emulator::GetFramebuffer() const {
    return _gpu.GetVRAM().GetRawData();
}

void Emulator::OnKeyDown(uint32_t keyCode) {
    if (keyCode == 0x1B) {
        Stop();
        return;
    }

    Pad& pad = _pad;
    PadMapper mapper;

    mapper.PressKey(pad, 0, keyCode);
}

void Emulator::OnKeyUp(uint32_t keyCode) {
    Pad& pad = _pad;
    PadMapper mapper;

    mapper.ReleaseKey(pad, 0, keyCode);
}

}