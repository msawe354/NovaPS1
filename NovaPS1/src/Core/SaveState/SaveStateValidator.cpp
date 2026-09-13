#include "SaveStateValidator.hpp"

namespace nova::ps1 {

SaveStateValidator::SaveStateValidator() {
}

uint32_t SaveStateValidator::ComputeChecksum(const std::vector<uint8_t>& data) const {
    uint32_t checksum = 0xFFFFFFFF;

    for (size_t i = 0; i < data.size(); i++) {
        checksum ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (checksum & 1) {
                checksum = (checksum >> 1) ^ 0xEDB88320;
            } else {
                checksum >>= 1;
            }
        }
    }

    return checksum ^ 0xFFFFFFFF;
}

bool SaveStateValidator::CheckMagic(const std::vector<uint8_t>& data) const {
    if (data.size() < 4) return false;

    uint32_t magic = static_cast<uint32_t>(data[0])
                   | (static_cast<uint32_t>(data[1]) << 8)
                   | (static_cast<uint32_t>(data[2]) << 16)
                   | (static_cast<uint32_t>(data[3]) << 24);

    return magic == GetExpectedMagic();
}

bool SaveStateValidator::CheckVersion(const std::vector<uint8_t>& data) const {
    if (data.size() < 8) return false;

    uint32_t version = static_cast<uint32_t>(data[4])
                     | (static_cast<uint32_t>(data[5]) << 8)
                     | (static_cast<uint32_t>(data[6]) << 16)
                     | (static_cast<uint32_t>(data[7]) << 24);

    return version == GetExpectedVersion();
}

bool SaveStateValidator::CheckSize(const std::vector<uint8_t>& data) const {
    return data.size() >= 12;
}

bool SaveStateValidator::Validate(const std::vector<uint8_t>& data) const {
    if (!CheckSize(data)) return false;
    if (!CheckMagic(data)) return false;
    if (!CheckVersion(data)) return false;
    return true;
}

}