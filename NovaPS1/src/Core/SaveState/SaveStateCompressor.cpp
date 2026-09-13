#include "SaveStateCompressor.hpp"

namespace nova::ps1 {

SaveStateCompressor::SaveStateCompressor() {
}

std::vector<uint8_t> SaveStateCompressor::RunLengthEncode(const std::vector<uint8_t>& input) {
    std::vector<uint8_t> output;

    if (input.empty()) return output;

    size_t i = 0;
    while (i < input.size()) {
        uint8_t value = input[i];
        size_t count = 1;

        while (i + count < input.size() && input[i + count] == value && count < 255) {
            count++;
        }

        if (count >= 4) {
            output.push_back(0xFF);
            output.push_back(static_cast<uint8_t>(count));
            output.push_back(value);
            i += count;
        } else {
            output.push_back(value);
            i++;
        }
    }

    return output;
}

std::vector<uint8_t> SaveStateCompressor::RunLengthDecode(const std::vector<uint8_t>& input) {
    std::vector<uint8_t> output;

    size_t i = 0;
    while (i < input.size()) {
        if (input[i] == 0xFF && i + 2 < input.size()) {
            uint8_t count = input[i + 1];
            uint8_t value = input[i + 2];
            for (uint8_t j = 0; j < count; j++) {
                output.push_back(value);
            }
            i += 3;
        } else {
            output.push_back(input[i]);
            i++;
        }
    }

    return output;
}

std::vector<uint8_t> SaveStateCompressor::Compress(const std::vector<uint8_t>& input) {
    return RunLengthEncode(input);
}

std::vector<uint8_t> SaveStateCompressor::Decompress(const std::vector<uint8_t>& input) {
    return RunLengthDecode(input);
}

}