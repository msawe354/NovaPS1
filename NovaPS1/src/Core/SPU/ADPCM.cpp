#include "ADPCM.hpp"

namespace nova::ps1 {

int16_t ADPCM::DecodeSample(uint8_t sample, uint8_t shift, uint8_t filter,
                            int32_t s1, int32_t s2) {
    if (shift > 12) shift = 12;

    int32_t nibble = static_cast<int32_t>(sample) << 12;
    nibble >>= shift;

    if (filter < 5) {
        nibble += (s1 * FILTER_TABLE[filter][0] + s2 * FILTER_TABLE[filter][1]) >> 6;
    }

    if (nibble > 32767) nibble = 32767;
    if (nibble < -32768) nibble = -32768;

    return static_cast<int16_t>(nibble);
}

}