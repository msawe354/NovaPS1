#pragma once
#include <cstdint>

namespace nova::ps1 {

class ADPCM {
public:
    static constexpr int32_t FILTER_TABLE[5][2] = {
        {0, 0},
        {60, 0},
        {115, -52},
        {98, -55},
        {122, -60}
    };

    static int16_t DecodeSample(uint8_t sample, uint8_t shift, uint8_t filter,
                                int32_t s1, int32_t s2);
};

}