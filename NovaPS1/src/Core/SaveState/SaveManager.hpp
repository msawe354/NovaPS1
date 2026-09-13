#pragma once
#include "SaveState.hpp"
#include <string>
#include <vector>

namespace nova::ps1 {

class Emulator;

class SaveManager {
public:
    SaveManager();
    ~SaveManager() = default;

    void Reset();
    void SetEmulator(Emulator* emu) { _emu = emu; }

    bool SaveSlot(uint32_t slot);
    bool LoadSlot(uint32_t slot);

    bool SaveToPath(const std::string& path);
    bool LoadFromPath(const std::string& path);

    uint32_t GetSlotCount() const { return _slotCount; }
    void SetSlotCount(uint32_t count) { _slotCount = count; }

    std::string GetSlotPath(uint32_t slot) const;

private:
    Emulator* _emu;
    SaveState _saveState;
    uint32_t _slotCount;
    std::string _basePath;
};

}