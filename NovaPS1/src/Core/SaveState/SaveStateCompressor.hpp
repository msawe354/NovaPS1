#pragma once
#include <cstdint>
#include <vector>

namespace nova::ps1 {

class SaveStateCompressor {
public:
    SaveStateCompressor();
    ~SaveStateCompressor() = default;

    std::vector<uint8_t> Compress(const std::vector<uint8_t>& input);
    std::vector<uint8_t> Decompress(const std::vector<uint8_t>& input);

private:
    std::vector<uint8_t> RunLengthEncode(const std::vector<uint8_t>& input);
    std::vector<uint8_t> RunLengthDecode(const std::vector<uint8_t>& input);
};

}