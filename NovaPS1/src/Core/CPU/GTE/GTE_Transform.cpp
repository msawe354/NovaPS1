#include "GTE_Transform.hpp"
#include "GTE.hpp"

namespace nova::ps1::gte {

static constexpr int32_t UNR = 0x7FFFFFFF;

static inline int64_t FixMul(int32_t a, int32_t b) {
    return static_cast<int64_t>(a) * static_cast<int64_t>(b);
}

static inline int32_t FixDiv(int64_t a, int64_t b) {
    if (b == 0) return UNR;
    return static_cast<int32_t>((a << 16) / b);
}

static void TransformVertex(GTE& gte, int vIndex) {
    uint32_t control = gte.ReadControl(0);
    int32_t r11 = static_cast<int16_t>(control & 0xFFFF);
    int32_t r12 = static_cast<int16_t>((control >> 16) & 0xFFFF);
    int32_t r13 = static_cast<int16_t>(gte.ReadControl(1) & 0xFFFF);
    int32_t r21 = static_cast<int16_t>((gte.ReadControl(1) >> 16) & 0xFFFF);
    int32_t r22 = static_cast<int16_t>(gte.ReadControl(2) & 0xFFFF);
    int32_t r23 = static_cast<int16_t>((gte.ReadControl(2) >> 16) & 0xFFFF);
    int32_t r31 = static_cast<int16_t>(gte.ReadControl(3) & 0xFFFF);
    int32_t r32 = static_cast<int16_t>((gte.ReadControl(3) >> 16) & 0xFFFF);
    int32_t r33 = static_cast<int16_t>(gte.ReadControl(4) & 0xFFFF);

    int32_t trX = static_cast<int32_t>(gte.ReadControl(5));
    int32_t trY = static_cast<int32_t>(gte.ReadControl(6));
    int32_t trZ = static_cast<int32_t>(gte.ReadControl(7));

    uint32_t vxy = gte.ReadData(0 + vIndex);
    uint32_t vz = gte.ReadData(4 + vIndex);

    int16_t vx = static_cast<int16_t>(vxy & 0xFFFF);
    int16_t vy = static_cast<int16_t>((vxy >> 16) & 0xFFFF);
    int16_t vz16 = static_cast<int16_t>(vz & 0xFFFF);

    int64_t mac1 = FixMul(r11, vx) + FixMul(r12, vy) + FixMul(r13, vz16) + (static_cast<int64_t>(trX) << 12);
    int64_t mac2 = FixMul(r21, vx) + FixMul(r22, vy) + FixMul(r23, vz16) + (static_cast<int64_t>(trY) << 12);
    int64_t mac3 = FixMul(r31, vx) + FixMul(r32, vy) + FixMul(r33, vz16) + (static_cast<int64_t>(trZ) << 12);

    gte.WriteData(25, static_cast<uint32_t>(mac1));
    gte.WriteData(26, static_cast<uint32_t>(mac2));
    gte.WriteData(27, static_cast<uint32_t>(mac3));

    int32_t ir1 = static_cast<int32_t>(mac1 >> 12);
    int32_t ir2 = static_cast<int32_t>(mac2 >> 12);
    int32_t ir3 = static_cast<int32_t>(mac3 >> 12);

    if (ir1 > 0x7FFF) { ir1 = 0x7FFF; }
    if (ir1 < -0x8000) { ir1 = -0x8000; }
    if (ir2 > 0x7FFF) { ir2 = 0x7FFF; }
    if (ir2 < -0x8000) { ir2 = -0x8000; }
    if (ir3 > 0x7FFF) { ir3 = 0x7FFF; }
    if (ir3 < -0x8000) { ir3 = -0x8000; }

    gte.WriteData(9, static_cast<uint32_t>(static_cast<uint16_t>(ir1)));
    gte.WriteData(10, static_cast<uint32_t>(static_cast<uint16_t>(ir2)));
    gte.WriteData(11, static_cast<uint32_t>(static_cast<uint16_t>(ir3)));

    int32_t h = static_cast<int32_t>(gte.ReadControl(26) & 0xFFFF);
    int32_t sz3 = static_cast<int32_t>(gte.ReadControl(19) & 0xFFFF);
    int32_t sz4 = static_cast<int32_t>(gte.ReadControl(20) & 0xFFFF);
    int64_t sz = FixMul(sz3, ir3) + sz4;

    int32_t sx = 0;
    int32_t sy = 0;

    if (h != 0 && sz != 0) {
        int64_t ofx = static_cast<int64_t>(static_cast<int32_t>(gte.ReadControl(24)));
        int64_t ofy = static_cast<int64_t>(static_cast<int32_t>(gte.ReadControl(25)));
        int64_t hMul = static_cast<int64_t>(h) << 16;

        int64_t x = (FixMul(sz, ir1) + ofx * 0x10000) / 0x10000;
        int64_t y = (FixMul(sz, ir2) + ofy * 0x10000) / 0x10000;

        sx = static_cast<int32_t>(x);
        sy = static_cast<int32_t>(y);

        if (sx > 0x3FF) sx = 0x3FF;
        if (sx < -0x400) sx = -0x400;
        if (sy > 0x3FF) sy = 0x3FF;
        if (sy < -0x400) sy = -0x400;

        (void)hMul;
    }

    gte.WriteData(12 + vIndex * 2, static_cast<uint32_t>(static_cast<uint16_t>(sx)));
    gte.WriteData(13 + vIndex * 2, static_cast<uint32_t>(static_cast<uint16_t>(sy)));

    int32_t z = static_cast<int32_t>(sz);
    if (z > 0xFFFF) z = 0xFFFF;
    if (z < 0) z = 0;
    gte.WriteData(16 + vIndex, static_cast<uint32_t>(z));

    int32_t ir0 = gte.ReadData(8);
    if (sz3 != 0) {
        int64_t div = FixDiv(static_cast<int64_t>(UNR), sz);
        ir0 = static_cast<int32_t>(div);
    }
    gte.WriteData(8, static_cast<uint32_t>(ir0));
}

void RTPS(GTE& gte, uint32_t opcode) {
    TransformVertex(gte, 0);
}

void RTPT(GTE& gte, uint32_t opcode) {
    TransformVertex(gte, 0);
    TransformVertex(gte, 1);
    TransformVertex(gte, 2);
}

}