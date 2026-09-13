#include "ExeLoader.hpp"
#include "../Bus/Bus.hpp"
#include <fstream>
#include <cstring>
#include <iostream>

namespace nova::ps1 {

bool ExeLoader::Load(const std::vector<uint8_t>& data, Bus& bus) {
    if (data.size() < 0x800) {
        std::cerr << "[ExeLoader] File too small" << std::endl;
        return false;
    }

    uint32_t magic = static_cast<uint32_t>(data[0])
                   | (static_cast<uint32_t>(data[1]) << 8)
                   | (static_cast<uint32_t>(data[2]) << 16)
                   | (static_cast<uint32_t>(data[3]) << 24);

    if (magic != PSX_EXE_MAGIC) {
        std::cerr << "[ExeLoader] Invalid magic: 0x" << std::hex << magic << std::dec << std::endl;
        return false;
    }

    uint32_t pc = static_cast<uint32_t>(data[0x10])
                | (static_cast<uint32_t>(data[0x11]) << 8)
                | (static_cast<uint32_t>(data[0x12]) << 16)
                | (static_cast<uint32_t>(data[0x13]) << 24);

    uint32_t gp = static_cast<uint32_t>(data[0x14])
                | (static_cast<uint32_t>(data[0x15]) << 8)
                | (static_cast<uint32_t>(data[0x16]) << 16)
                | (static_cast<uint32_t>(data[0x17]) << 24);

    uint32_t loadAddr = static_cast<uint32_t>(data[0x18])
                      | (static_cast<uint32_t>(data[0x19]) << 8)
                      | (static_cast<uint32_t>(data[0x1A]) << 16)
                      | (static_cast<uint32_t>(data[0x1B]) << 24);

    uint32_t loadSize = static_cast<uint32_t>(data[0x1C])
                      | (static_cast<uint32_t>(data[0x1D]) << 8)
                      | (static_cast<uint32_t>(data[0x1E]) << 16)
                      | (static_cast<uint32_t>(data[0x1F]) << 24);

    for (uint32_t i = 0; i < loadSize && (0x800 + i) < data.size(); i++) {
        bus.Write8(loadAddr + i, data[0x800 + i]);
    }

    std::cout << "[ExeLoader] PC=0x" << std::hex << pc
              << " GP=0x" << gp
              << " Load=0x" << loadAddr
              << " Size=0x" << loadSize << std::dec << std::endl;

    return true;
}

bool ExeLoader::LoadFromFile(const std::string& path, Bus& bus) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "[ExeLoader] Cannot open: " << path << std::endl;
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> data(static_cast<size_t>(size));
    file.read(reinterpret_cast<char*>(data.data()), size);

    return Load(data, bus);
}

}