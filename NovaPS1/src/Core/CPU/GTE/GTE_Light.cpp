#include "GTE_Light.hpp"
#include "GTE.hpp"

namespace nova::ps1::gte {

static inline int64_t FixMul(int32_t a, int32_t b) {
    return static_cast<int64_t>(a) * static_cast<int64_t>(b);
}

static inline int16_t ClampIR(int64_t value) {
    if (value > 0x7FFF) return 0x7FFF;
    if (value < -0x8000) return static_cast<int16_t>(-0x8000);
    return static_cast<int16_t>(value);
}

static inline int16_t ClampColor(int64_t value) {
    if (value > 0xFF) return 0xFF;
    if (value < 0) return 0;
    return static_cast<int16_t>(value);
}

static void LoadLightMatrix(GTE& gte, int32_t& l11, int32_t& l12, int32_t& l13,
                             int32_t& l21, int32_t& l22, int32_t& l23,
                             int32_t& l31, int32_t& l32, int32_t& l33) {
    uint32_t c8  = gte.ReadControl(8);
    uint32_t c9  = gte.ReadControl(9);
    uint32_t c10 = gte.ReadControl(10);
    uint32_t c11 = gte.ReadControl(11);
    uint32_t c12 = gte.ReadControl(12);

    l11 = static_cast<int16_t>(c8 & 0xFFFF);
    l12 = static_cast<int16_t>((c8 >> 16) & 0xFFFF);
    l13 = static_cast<int16_t>(c9 & 0xFFFF);
    l21 = static_cast<int16_t>((c9 >> 16) & 0xFFFF);
    l22 = static_cast<int16_t>(c10 & 0xFFFF);
    l23 = static_cast<int16_t>((c10 >> 16) & 0xFFFF);
    l31 = static_cast<int16_t>(c11 & 0xFFFF);
    l32 = static_cast<int16_t>((c11 >> 16) & 0xFFFF);
    l33 = static_cast<int16_t>(c12 & 0xFFFF);
}

static void LoadBackgroundColor(GTE& gte, int32_t& rbk, int32_t& gbk, int32_t& bbk) {
    uint32_t c13 = gte.ReadControl(13);
    uint32_t c14 = gte.ReadControl(14);
    rbk = static_cast<int32_t>(c13 & 0xFFFF);
    gbk = static_cast<int32_t>(c13 >> 16);
    bbk = static_cast<int32_t>(c14 & 0xFFFF);
}

static void ComputeVertexNormal(GTE& gte, int vIndex,
                                int32_t l11, int32_t l12, int32_t l13,
                                int32_t l21, int32_t l22, int32_t l23,
                                int32_t l31, int32_t l32, int32_t l33) {
    uint32_t vxy = gte.ReadData(0 + vIndex);
    uint32_t vz = gte.ReadData(4 + vIndex);

    int16_t vx = static_cast<int16_t>(vxy & 0xFFFF);
    int16_t vy = static_cast<int16_t>((vxy >> 16) & 0xFFFF);
    int16_t vz16 = static_cast<int16_t>(vz & 0xFFFF);

    int64_t mac1 = FixMul(l11, vx) + FixMul(l12, vy) + FixMul(l13, vz16);
    int64_t mac2 = FixMul(l21, vx) + FixMul(l22, vy) + FixMul(l23, vz16);
    int64_t mac3 = FixMul(l31, vx) + FixMul(l32, vy) + FixMul(l33, vz16);

    gte.WriteData(25, static_cast<uint32_t>(mac1));
    gte.WriteData(26, static_cast<uint32_t>(mac2));
    gte.WriteData(27, static_cast<uint32_t>(mac3));

    int32_t ir1 = static_cast<int32_t>(mac1 >> 12);
    int32_t ir2 = static_cast<int32_t>(mac2 >> 12);
    int32_t ir3 = static_cast<int32_t>(mac3 >> 12);

    gte.WriteData(9, static_cast<uint32_t>(static_cast<uint16_t>(ClampIR(ir1))));
    gte.WriteData(10, static_cast<uint32_t>(static_cast<uint16_t>(ClampIR(ir2))));
    gte.WriteData(11, static_cast<uint32_t>(static_cast<uint16_t>(ClampIR(ir3))));
}

static void ComputeColor(GTE& gte, int32_t rbk, int32_t gbk, int32_t bbk,
                         int32_t lr1, int32_t lr2, int32_t lr3,
                         int32_t lg1, int32_t lg2, int32_t lg3,
                         int32_t lb1, int32_t lb2, int32_t lb3,
                         int32_t addR, int32_t addG, int32_t addB) {
    int32_t ir1 = static_cast<int16_t>(gte.ReadData(9) & 0xFFFF);
    int32_t ir2 = static_cast<int16_t>(gte.ReadData(10) & 0xFFFF);
    int32_t ir3 = static_cast<int16_t>(gte.ReadData(11) & 0xFFFF);

    int64_t mac1 = (static_cast<int64_t>(rbk) << 12) + FixMul(lr1, ir1) + FixMul(lr2, ir2) + FixMul(lr3, ir3) + (static_cast<int64_t>(addR) << 12);
    int64_t mac2 = (static_cast<int64_t>(gbk) << 12) + FixMul(lg1, ir1) + FixMul(lg2, ir2) + FixMul(lg3, ir3) + (static_cast<int64_t>(addG) << 12);
    int64_t mac3 = (static_cast<int64_t>(bbk) << 12) + FixMul(lb1, ir1) + FixMul(lb2, ir2) + FixMul(lb3, ir3) + (static_cast<int64_t>(addB) << 12);

    int32_t r = static_cast<int32_t>(mac1 >> 12);
    int32_t g = static_cast<int32_t>(mac2 >> 12);
    int32_t b = static_cast<int32_t>(mac3 >> 12);

    gte.WriteData(22, static_cast<uint32_t>(static_cast<uint16_t>(ClampColor(r))));
    gte.WriteData(23, static_cast<uint32_t>(static_cast<uint16_t>(ClampColor(g))));
    gte.WriteData(24, static_cast<uint32_t>(static_cast<uint16_t>(ClampColor(b))));
}

void NCS(GTE& gte, uint32_t opcode) {
    int32_t l11, l12, l13, l21, l22, l23, l31, l32, l33;
    LoadLightMatrix(gte, l11, l12, l13, l21, l22, l23, l31, l32, l33);

    int32_t rbk, gbk, bbk;
    LoadBackgroundColor(gte, rbk, gbk, bbk);

    ComputeVertexNormal(gte, 0, l11, l12, l13, l21, l22, l23, l31, l32, l33);

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

    ComputeColor(gte, rbk, gbk, bbk, lr1, lr2, lr3, lg1, lg2, lg3, lb1, lb2, lb3, 0, 0, 0);
}

void NCT(GTE& gte, uint32_t opcode) {
    int32_t l11, l12, l13, l21, l22, l23, l31, l32, l33;
    LoadLightMatrix(gte, l11, l12, l13, l21, l22, l23, l31, l32, l33);

    int32_t rbk, gbk, bbk;
    LoadBackgroundColor(gte, rbk, gbk, bbk);

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

    for (int i = 0; i < 3; i++) {
        ComputeVertexNormal(gte, i, l11, l12, l13, l21, l22, l23, l31, l32, l33);
        ComputeColor(gte, rbk, gbk, bbk, lr1, lr2, lr3, lg1, lg2, lg3, lb1, lb2, lb3, 0, 0, 0);

        uint32_t color = (gte.ReadData(22) & 0xFF) | ((gte.ReadData(23) & 0xFF) << 8) | ((gte.ReadData(24) & 0xFF) << 16);
        gte.WriteData(20 + i, color);
    }
}

void NCDS(GTE& gte, uint32_t opcode) {
    int32_t l11, l12, l13, l21, l22, l23, l31, l32, l33;
    LoadLightMatrix(gte, l11, l12, l13, l21, l22, l23, l31, l32, l33);

    int32_t rbk, gbk, bbk;
    LoadBackgroundColor(gte, rbk, gbk, bbk);

    ComputeVertexNormal(gte, 0, l11, l12, l13, l21, l22, l23, l31, l32, l33);

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

    uint32_t vxy = gte.ReadData(6);
    int16_t vx = static_cast<int16_t>(vxy & 0xFFFF);
    int16_t vy = static_cast<int16_t>((vxy >> 16) & 0xFFFF);
    int16_t vz = static_cast<int16_t>(gte.ReadData(6 + 0) & 0xFFFF);

    int64_t mac1 = FixMul(vx, gte.ReadIR(1));
    int64_t mac2 = FixMul(vy, gte.ReadIR(2));
    int64_t mac3 = FixMul(vz, gte.ReadIR(3));

    int32_t ir0 = static_cast<int32_t>(mac1 >> 12);
    if (ir0 > 0x1000) ir0 = 0x1000;
    if (ir0 < 0) ir0 = 0;
    gte.WriteData(8, static_cast<uint32_t>(ir0));

    int32_t ir1 = static_cast<int32_t>(mac1 >> 12);
    int32_t ir2 = static_cast<int32_t>(mac2 >> 12);
    int32_t ir3 = static_cast<int32_t>(mac3 >> 12);

    int32_t addR = (ir1 * ir0) >> 12;
    int32_t addG = (ir2 * ir0) >> 12;
    int32_t addB = (ir3 * ir0) >> 12;

    ComputeColor(gte, rbk, gbk, bbk, lr1, lr2, lr3, lg1, lg2, lg3, lb1, lb2, lb3, addR, addG, addB);
}

void NCDT(GTE& gte, uint32_t opcode) {
    for (int i = 0; i < 3; i++) {
        NCDS(gte, opcode);

        uint32_t color = (gte.ReadData(22) & 0xFF) | ((gte.ReadData(23) & 0xFF) << 8) | ((gte.ReadData(24) & 0xFF) << 16);
        gte.WriteData(20 + i, color);
    }
}

void NCCS(GTE& gte, uint32_t opcode) {
    NCS(gte, opcode);
}

void NCCT(GTE& gte, uint32_t opcode) {
    NCT(gte, opcode);
}

void CC(GTE& gte, uint32_t opcode) {
    NCS(gte, opcode);
}

void CDP(GTE& gte, uint32_t opcode) {
    NCDS(gte, opcode);
}

}