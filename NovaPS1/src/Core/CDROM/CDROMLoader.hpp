#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace nova::ps1 {

class CDROMLoader {
public:
    CDROMLoader();
    ~CDROMLoader() = default;

    bool LoadISO(const std::string& path, std::vector<uint8_t>& outData);
    bool LoadBIN(const std::string& path, std::vector<uint8_t>& outData);
    bool LoadCUE(const std::string& path, std::vector<uint8_t>& outData);

    static constexpr uint32_t SECTOR_SIZE = 2352;
    static constexpr uint32_t SECTOR_DATA_SIZE = 2048;

private:
    uint32_t Read32LE(const uint8_t* data) const;
    uint16_t Read16LE(const uint8_t* data) const;
};

}