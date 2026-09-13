#include "SaveManager.hpp"
#include "../Emulator.hpp"

namespace nova::ps1 {

SaveManager::SaveManager()
    : _emu(nullptr),
      _slotCount(10),
      _basePath("saves/") {
}

void SaveManager::Reset() {
    _saveState.Reset();
}

std::string SaveManager::GetSlotPath(uint32_t slot) const {
    return _basePath + "slot" + std::to_string(slot) + ".state";
}

bool SaveManager::SaveSlot(uint32_t slot) {
    if (!_emu) return false;
    if (slot >= _slotCount) return false;

    return _saveState.Save(*_emu, GetSlotPath(slot));
}

bool SaveManager::LoadSlot(uint32_t slot) {
    if (!_emu) return false;
    if (slot >= _slotCount) return false;

    return _saveState.Load(*_emu, GetSlotPath(slot));
}

bool SaveManager::SaveToPath(const std::string& path) {
    if (!_emu) return false;
    return _saveState.Save(*_emu, path);
}

bool SaveManager::LoadFromPath(const std::string& path) {
    if (!_emu) return false;
    return _saveState.Load(*_emu, path);
}

}