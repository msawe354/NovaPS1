#include "GTE_Color.hpp"
#include "GTE.hpp"

namespace nova::ps1::gte {

static inline int64_t FixMul(int32_t a, int32_t b) {
    return static_cast<int64_t>(a) * static_cast<int64_t>(b);
}

static inline int16_t ClampColor(int64_t value) {
    if (value > 0xFF) return 0xFF;
    if (value < 0) return 0;
    return static_cast<int16_t>(value);
}

static void InterpolateColor(GTE& gte, int32_t ir1, int32_t ir2, int32_t ir3, int32_t ir0,
                             int32_t addR, int32_t addG, int32_t addB) {
    uint32_t c16 = gte.ReadControl(16);
    uint32_t c17 = gte.ReadControl(17);
    uint32_t c18 = gte.ReadControl(18);

    int32_t lr1 = static_cast<int16_t>(c16 & 0xFFFF);
    int32_t lr2 = static_cast<int16_t>((c16 >> 16) & 0xFFFF);
    int32_t lr3 = static_cast<int16_t>(c17 & 0xFFFF);
    int32_t lg1 = static_cast<int16_t>((c17 >> 16) & 0xFFFF);
    int32_t lg2 = static_cast<int16_t>(c18 & 0xFFFF);
    int32_t lg3 = static_cast<int16_t>((c18 >> 16) & 0xFFFF);
    int32_t lb1 = static_cast<int16_t>(gte.ReadControl(19) & 0xFFFF);
    int32_t lb2 = static_cast<int16_t>((gte.ReadControl(19) >> 16) & 0xFFFF);
    int32_t lb3 = static_cast<int16_t>(gte.ReadControl(20) & 0xFFFF);

    int64_t mac1 = (static_cast<int64_t>(lr1) * ir1 + static_cast<int64_t>(lr2) * ir2 + static_cast<int64_t>(lr3) * ir3 + (static_cast<int64_t>(addR) << 12)) >> 12;
    int64_t mac2 = (static_cast<int64_t>(lg1) * ir1 + static_cast<int64_t>(lg2) * ir2 + static_cast<int64_t>(lg3) * ir3 + (static_cast<int64_t>(addG) << 12)) >> 12;
    int64_t mac3 = (static_cast<int64_t>(lb1) * ir1 + static_cast<int64_t>(lb2) * ir2 + static_cast<int64_t>(lb3) * ir3 + (static_cast<int64_t>(addB) << 12)) >> 12;

    int32_t r = static_cast<int32_t>(mac1);
    int32_t g = static_cast<int32_t>(mac2);
    int32_t b = static_cast<int32_t>(mac3);

    (void)ir0;

    gte.WriteData(22, static_cast<uint32_t>(static_cast<uint16_t>(ClampColor(r))));
    gte.WriteData(23, static_cast<uint32_t>(static_cast<uint16_t>(ClampColor(g))));
    gte.WriteData(24, static_cast<uint32_t>(static_cast<uint16_t>(ClampColor(b))));
}

void DPCS(GTE& gte, uint32_t opcode) {
    uint32_t rgb = gte.ReadData(6);

    int32_t r = static_cast<int32_t>(rgb & 0xFF);
    int32_t g = static_cast<int32_t>((rgb >> 8) & 0xFF);
    int32_t b = static_cast<int32_t>((rgb >> 16) & 0xFF);

    int32_t ir0 = static_cast<int32_t>(gte.ReadData(8));

    int32_t addR = (r << 16) >> 12;
    int32_t addG = (g << 16) >> 12;
    int32_t addB = (b << 16) >> 12;

    InterpolateColor(gte, ir0, ir0, ir0, ir0, addR, addG, addB);
}

void DPCT(GTE& gte, uint32_t opcode) {
    for (int i = 0; i < 3; i++) {
        DPCS(gte, opcode);

        uint32_t color = (gte.ReadData(22) & 0xFF) | ((gte.ReadData(23) & 0xFF) << 8) | ((gte.ReadData(24) & 0xFF) << 16);
        gte.WriteData(20 + i, color);
    }
}

void DCPL(GTE& gte, uint32_t opcode) {
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

    InterpolateColor(gte, ir1, ir2, ir3, 0, addR, addG, addB);
}

void INTPL(GTE& gte, uint32_t opcode) {
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

    int64_t mac1 = (FixMul(ir1, 0x1000) + (static_cast<int64_t>(addR) << 12)) >> 12;
    int64_t mac2 = (FixMul(ir2, 0x1000) + (static_cast<int64_t>(addG) << 12)) >> 12;
    int64_t mac3 = (FixMul(ir3, 0x1000) + (static_cast<int64_t>(addB) << 12)) >> 12;

    gte.WriteData(22, static_cast<uint32_t>(static_cast<uint16_t>(ClampColor(mac1))));
    gte.WriteData(23, static_cast<uint32_t>(static_cast<uint16_t>(ClampColor(mac2))));
    gte.WriteData(24, static_cast<uint32_t>(static_cast<uint16_t>(ClampColor(mac3))));
}

}