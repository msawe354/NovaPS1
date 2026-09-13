#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace nova::ps1 {

class Bus;

class ExeLoader {
public:
    ExeLoader() = default;
    ~ExeLoader() = default;

    bool Load(const std::vector<uint8_t>& data, Bus& bus);
    bool LoadFromFile(const std::string& path, Bus& bus);

private:
    static constexpr uint32_t PSX_EXE_MAGIC = 0x45584550;
};

}