#pragma once
#include <cstdint>
#include <vector>

namespace nova::ps1 {

class SaveStateValidator {
public:
    SaveStateValidator();
    ~SaveStateValidator() = default;

    bool Validate(const std::vector<uint8_t>& data) const;

    uint32_t ComputeChecksum(const std::vector<uint8_t>& data) const;

    uint32_t GetExpectedMagic() const { return 0x4E4F5641; }
    uint32_t GetExpectedVersion() const { return 1; }

private:
    bool CheckMagic(const std::vector<uint8_t>& data) const;
    bool CheckVersion(const std::vector<uint8_t>& data) const;
    bool CheckSize(const std::vector<uint8_t>& data) const;
};

}