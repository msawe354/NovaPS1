#include "CDROMLoader.hpp"
#include <fstream>
#include <iostream>

namespace nova::ps1 {

CDROMLoader::CDROMLoader() {
}

uint32_t CDROMLoader::Read32LE(const uint8_t* data) const {
    return static_cast<uint32_t>(data[0])
         | (static_cast<uint32_t>(data[1]) << 8)
         | (static_cast<uint32_t>(data[2]) << 16)
         | (static_cast<uint32_t>(data[3]) << 24);
}

uint16_t CDROMLoader::Read16LE(const uint8_t* data) const {
    return static_cast<uint16_t>(data[0] | (data[1] << 8));
}

bool CDROMLoader::LoadISO(const std::string& path, std::vector<uint8_t>& outData) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "[CDROMLoader] Cannot open ISO: " << path << std::endl;
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    outData.resize(static_cast<size_t>(size));
    file.read(reinterpret_cast<char*>(outData.data()), size);
    file.close();

    std::cout << "[CDROMLoader] ISO loaded: " << path << " (" << size << " bytes)" << std::endl;
    return true;
}

bool CDROMLoader::LoadBIN(const std::string& path, std::vector<uint8_t>& outData) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "[CDROMLoader] Cannot open BIN: " << path << std::endl;
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    outData.resize(static_cast<size_t>(size));
    file.read(reinterpret_cast<char*>(outData.data()), size);
    file.close();

    std::cout << "[CDROMLoader] BIN loaded: " << path << " (" << size << " bytes)" << std::endl;
    return true;
}

bool CDROMLoader::LoadCUE(const std::string& path, std::vector<uint8_t>& outData) {
    std::ifstream cue(path);
    if (!cue.is_open()) {
        std::cerr << "[CDROMLoader] Cannot open CUE: " << path << std::endl;
        return false;
    }

    std::string line;
    std::string binPath;

    while (std::getline(cue, line)) {
        if (line.find("FILE") != std::string::npos) {
            size_t first = line.find('"');
            size_t last = line.find('"', first + 1);
            if (first != std::string::npos && last != std::string::npos) {
                binPath = line.substr(first + 1, last - first - 1);
            }
        }
    }

    cue.close();

    if (binPath.empty()) {
        std::cerr << "[CDROMLoader] No BIN file found in CUE" << std::endl;
        return false;
    }

    size_t slash = path.find_last_of("/\\");
    std::string dir = (slash != std::string::npos) ? path.substr(0, slash + 1) : "";

    return LoadBIN(dir + binPath, outData);
}

}