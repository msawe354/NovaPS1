#include "GTE_Depth.hpp"
#include "GTE.hpp"

namespace nova::ps1::gte {

static inline int16_t ClampColor(int64_t value) {
    if (value > 0xFF) return 0xFF;
    if (value < 0) return 0;
    return static_cast<int16_t>(value);
}

void DPCL(GTE& gte, uint32_t opcode) {
    uint32_t rgb = gte.ReadData(6);

    int32_t r = static_cast<int32_t>(rgb & 0xFF);
    int32_t g = static_cast<int32_t>((rgb >> 8) & 0xFF);
    int32_t b = static_cast<int32_t>((rgb >> 16) & 0xFF);

    int32_t ir1 = static_cast<int16_t>(gte.ReadData(9) & 0xFFFF);
    int32_t ir2 = static_cast<int16_t>(gte.ReadData(10) & 0xFFFF);
    int32_t ir3 = static_cast<int16_t>(gte.ReadData(11) & 0xFFFF);

    int32_t addR = (r << 16) >> 12;
    int32_t addG = (g << 16) >> 12;
    int32_t addB = (b << 16) >> 12;

    int64_t mac1 = (static_cast<int64_t>(ir1) * 0x1000 + (static_cast<int64_t>(addR) << 12)) >> 12;
    int64_t mac2 = (static_cast<int64_t>(ir2) * 0x1000 + (static_cast<int64_t>(addG) << 12)) >> 12;
    int64_t mac3 = (static_cast<int64_t>(ir3) * 0x1000 + (static_cast<int64_t>(addB) << 12)) >> 12;

    gte.WriteData(22, static_cast<uint32_t>(static_cast<uint16_t>(ClampColor(mac1))));
    gte.WriteData(23, static_cast<uint32_t>(static_cast<uint16_t>(ClampColor(mac2))));
    gte.WriteData(24, static_cast<uint32_t>(static_cast<uint16_t>(ClampColor(mac3))));
}

void DPST(GTE& gte, uint32_t opcode) {
    for (int i = 0; i < 3; i++) {
        DPCL(gte, opcode);

        uint32_t color = (gte.ReadData(22) & 0xFF) | ((gte.ReadData(23) & 0xFF) << 8) | ((gte.ReadData(24) & 0xFF) << 16);
        gte.WriteData(20 + i, color);
    }
}

void AVSZ3(GTE& gte, uint32_t opcode) {
    uint32_t z1 = gte.ReadData(17) & 0xFFFF;
    uint32_t z2 = gte.ReadData(18) & 0xFFFF;
    uint32_t z3 = gte.ReadData(19) & 0xFFFF;

    int32_t sz3 = static_cast<int32_t>(gte.ReadControl(29) & 0xFFFF);

    int64_t sum = static_cast<int64_t>(z1) + z2 + z3;
    int64_t mac0 = sum * sz3;

    gte.WriteData(24, static_cast<uint32_t>(static_cast<int32_t>(mac0)));

    int32_t otz = static_cast<int32_t>(mac0 >> 12);
    if (otz > 0xFFFF) otz = 0xFFFF;
    if (otz < 0) otz = 0;
    gte.WriteData(7, static_cast<uint32_t>(otz));
}

void AVSZ4(GTE& gte, uint32_t opcode) {
    uint32_t z0 = gte.ReadData(16) & 0xFFFF;
    uint32_t z1 = gte.ReadData(17) & 0xFFFF;
    uint32_t z2 = gte.ReadData(18) & 0xFFFF;
    uint32_t z3 = gte.ReadData(19) & 0xFFFF;

    int32_t sz4 = static_cast<int32_t>(gte.ReadControl(30) & 0xFFFF);

    int64_t sum = static_cast<int64_t>(z0) + z1 + z2 + z3;
    int64_t mac0 = sum * sz4;

    gte.WriteData(24, static_cast<uint32_t>(static_cast<int32_t>(mac0)));

    int32_t otz = static_cast<int32_t>(mac0 >> 12);
    if (otz > 0xFFFF) otz = 0xFFFF;
    if (otz < 0) otz = 0;
    gte.WriteData(7, static_cast<uint32_t>(otz));
}

}