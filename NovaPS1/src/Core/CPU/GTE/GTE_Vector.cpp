#include "GTE_Vector.hpp"
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

void NCLIP(GTE& gte, uint32_t opcode) {
    int32_t sx0 = static_cast<int16_t>(gte.ReadData(12) & 0xFFFF);
    int32_t sy0 = static_cast<int16_t>(gte.ReadData(13) & 0xFFFF);
    int32_t sx1 = static_cast<int16_t>(gte.ReadData(14) & 0xFFFF);
    int32_t sy1 = static_cast<int16_t>(gte.ReadData(15) & 0xFFFF);
    int32_t sx2 = static_cast<int16_t>(gte.ReadData(16) & 0xFFFF);
    int32_t sy2 = static_cast<int16_t>(gte.ReadData(17) & 0xFFFF);

    int64_t mac0 = FixMul(sx0, sy1) + FixMul(sx1, sy2) + FixMul(sx2, sy0)
                 - FixMul(sx0, sy2) - FixMul(sx1, sy0) - FixMul(sx2, sy1);

    gte.WriteData(24, static_cast<uint32_t>(static_cast<int32_t>(mac0)));
}

void MVMVA(GTE& gte, uint32_t opcode) {
    uint32_t mx = (opcode >> 17) & 0x3;
    uint32_t vx = (opcode >> 15) & 0x3;
    uint32_t tx = (opcode >> 13) & 0x3;

    int32_t m11, m12, m13, m21, m22, m23, m31, m32, m33;

    switch (mx) {
        case 0: {
            uint32_t c0 = gte.ReadControl(0);
            uint32_t c1 = gte.ReadControl(1);
            uint32_t c2 = gte.ReadControl(2);
            m11 = static_cast<int16_t>(c0 & 0xFFFF);
            m12 = static_cast<int16_t>((c0 >> 16) & 0xFFFF);
            m13 = static_cast<int16_t>(c1 & 0xFFFF);
            m21 = static_cast<int16_t>((c1 >> 16) & 0xFFFF);
            m22 = static_cast<int16_t>(c2 & 0xFFFF);
            m23 = static_cast<int16_t>((c2 >> 16) & 0xFFFF);
            m31 = static_cast<int16_t>(gte.ReadControl(3) & 0xFFFF);
            m32 = static_cast<int16_t>((gte.ReadControl(3) >> 16) & 0xFFFF);
            m33 = static_cast<int16_t>(gte.ReadControl(4) & 0xFFFF);
            break;
        }
        case 1: {
            uint32_t c8 = gte.ReadControl(8);
            uint32_t c9 = gte.ReadControl(9);
            m11 = static_cast<int16_t>(c8 & 0xFFFF);
            m12 = static_cast<int16_t>((c8 >> 16) & 0xFFFF);
            m13 = static_cast<int16_t>(c9 & 0xFFFF);
            m21 = static_cast<int16_t>((c9 >> 16) & 0xFFFF);
            m22 = static_cast<int16_t>(gte.ReadControl(10) & 0xFFFF);
            m23 = static_cast<int16_t>((gte.ReadControl(10) >> 16) & 0xFFFF);
            m31 = static_cast<int16_t>(gte.ReadControl(11) & 0xFFFF);
            m32 = static_cast<int16_t>((gte.ReadControl(11) >> 16) & 0xFFFF);
            m33 = static_cast<int16_t>(gte.ReadControl(12) & 0xFFFF);
            break;
        }
        case 2: {
            uint32_t c16 = gte.ReadControl(16);
            uint32_t c17 = gte.ReadControl(17);
            m11 = static_cast<int16_t>(c16 & 0xFFFF);
            m12 = static_cast<int16_t>((c16 >> 16) & 0xFFFF);
            m13 = static_cast<int16_t>(c17 & 0xFFFF);
            m21 = static_cast<int16_t>((c17 >> 16) & 0xFFFF);
            m22 = static_cast<int16_t>(gte.ReadControl(18) & 0xFFFF);
            m23 = static_cast<int16_t>((gte.ReadControl(18) >> 16) & 0xFFFF);
            m31 = static_cast<int16_t>(gte.ReadControl(19) & 0xFFFF);
            m32 = static_cast<int16_t>((gte.ReadControl(19) >> 16) & 0xFFFF);
            m33 = static_cast<int16_t>(gte.ReadControl(20) & 0xFFFF);
            break;
        }
        default:
            m11 = m12 = m13 = 0;
            m21 = m22 = m23 = 0;
            m31 = m32 = m33 = 0;
            break;
    }

    int32_t vx0, vy0, vz0;
    switch (vx) {
        case 0:
            vx0 = static_cast<int16_t>(gte.ReadData(0) & 0xFFFF);
            vy0 = static_cast<int16_t>((gte.ReadData(0) >> 16) & 0xFFFF);
            vz0 = static_cast<int16_t>(gte.ReadData(4) & 0xFFFF);
            break;
        case 1:
            vx0 = static_cast<int16_t>(gte.ReadData(1) & 0xFFFF);
            vy0 = static_cast<int16_t>((gte.ReadData(1) >> 16) & 0xFFFF);
            vz0 = static_cast<int16_t>(gte.ReadData(5) & 0xFFFF);
            break;
        case 2:
            vx0 = static_cast<int16_t>(gte.ReadData(2) & 0xFFFF);
            vy0 = static_cast<int16_t>((gte.ReadData(2) >> 16) & 0xFFFF);
            vz0 = static_cast<int16_t>(gte.ReadData(6) & 0xFFFF);
            break;
        default:
            vx0 = vy0 = vz0 = 0;
            break;
    }

    int32_t tx0 = 0, ty0 = 0, tz0 = 0;
    if (tx == 0) {
        tx0 = static_cast<int32_t>(gte.ReadControl(5));
        ty0 = static_cast<int32_t>(gte.ReadControl(6));
        tz0 = static_cast<int32_t>(gte.ReadControl(7));
    } else if (tx == 1) {
        tx0 = static_cast<int32_t>(gte.ReadControl(13) & 0xFFFF);
        ty0 = static_cast<int32_t>(gte.ReadControl(13) >> 16);
        tz0 = static_cast<int32_t>(gte.ReadControl(14) & 0xFFFF);
    }

    int64_t mac1 = FixMul(m11, vx0) + FixMul(m12, vy0) + FixMul(m13, vz0) + (static_cast<int64_t>(tx0) << 12);
    int64_t mac2 = FixMul(m21, vx0) + FixMul(m22, vy0) + FixMul(m23, vz0) + (static_cast<int64_t>(ty0) << 12);
    int64_t mac3 = FixMul(m31, vx0) + FixMul(m32, vy0) + FixMul(m33, vz0) + (static_cast<int64_t>(tz0) << 12);

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

void OP(GTE& gte, uint32_t opcode) {
    int32_t ir1 = static_cast<int16_t>(gte.ReadData(9) & 0xFFFF);
    int32_t ir2 = static_cast<int16_t>(gte.ReadData(10) & 0xFFFF);
    int32_t ir3 = static_cast<int16_t>(gte.ReadData(11) & 0xFFFF);

    uint32_t c0 = gte.ReadControl(0);
    uint32_t c1 = gte.ReadControl(1);
    uint32_t c2 = gte.ReadControl(2);

    int32_t m11 = static_cast<int16_t>(c0 & 0xFFFF);
    int32_t m12 = static_cast<int16_t>((c0 >> 16) & 0xFFFF);
    int32_t m13 = static_cast<int16_t>(c1 & 0xFFFF);
    int32_t m21 = static_cast<int16_t>((c1 >> 16) & 0xFFFF);
    int32_t m22 = static_cast<int16_t>(c2 & 0xFFFF);
    int32_t m23 = static_cast<int16_t>((c2 >> 16) & 0xFFFF);
    int32_t m31 = static_cast<int16_t>(gte.ReadControl(3) & 0xFFFF);
    int32_t m32 = static_cast<int16_t>((gte.ReadControl(3) >> 16) & 0xFFFF);
    int32_t m33 = static_cast<int16_t>(gte.ReadControl(4) & 0xFFFF);

    int32_t trX = static_cast<int32_t>(gte.ReadControl(5));
    int32_t trY = static_cast<int32_t>(gte.ReadControl(6));
    int32_t trZ = static_cast<int32_t>(gte.ReadControl(7));

    int64_t mac1 = FixMul(m11, ir1) + FixMul(m12, ir2) + FixMul(m13, ir3) + (static_cast<int64_t>(trX) << 12);
    int64_t mac2 = FixMul(m21, ir1) + FixMul(m22, ir2) + FixMul(m23, ir3) + (static_cast<int64_t>(trY) << 12);
    int64_t mac3 = FixMul(m31, ir1) + FixMul(m32, ir2) + FixMul(m33, ir3) + (static_cast<int64_t>(trZ) << 12);

    gte.WriteData(25, static_cast<uint32_t>(mac1));
    gte.WriteData(26, static_cast<uint32_t>(mac2));
    gte.WriteData(27, static_cast<uint32_t>(mac3));

    int32_t oir1 = static_cast<int32_t>(mac1 >> 12);
    int32_t oir2 = static_cast<int32_t>(mac2 >> 12);
    int32_t oir3 = static_cast<int32_t>(mac3 >> 12);

    gte.WriteData(9, static_cast<uint32_t>(static_cast<uint16_t>(ClampIR(oir1))));
    gte.WriteData(10, static_cast<uint32_t>(static_cast<uint16_t>(ClampIR(oir2))));
    gte.WriteData(11, static_cast<uint32_t>(static_cast<uint16_t>(ClampIR(oir3))));
}

void SQR(GTE& gte, uint32_t opcode) {
    int32_t ir1 = static_cast<int16_t>(gte.ReadData(9) & 0xFFFF);
    int32_t ir2 = static_cast<int16_t>(gte.ReadData(10) & 0xFFFF);
    int32_t ir3 = static_cast<int16_t>(gte.ReadData(11) & 0xFFFF);

    int64_t mac1 = FixMul(ir1, ir1) >> 12;
    int64_t mac2 = FixMul(ir2, ir2) >> 12;
    int64_t mac3 = FixMul(ir3, ir3) >> 12;

    gte.WriteData(25, static_cast<uint32_t>(mac1));
    gte.WriteData(26, static_cast<uint32_t>(mac2));
    gte.WriteData(27, static_cast<uint32_t>(mac3));

    gte.WriteData(9, static_cast<uint32_t>(static_cast<uint16_t>(ClampIR(mac1))));
    gte.WriteData(10, static_cast<uint32_t>(static_cast<uint16_t>(ClampIR(mac2))));
    gte.WriteData(11, static_cast<uint32_t>(static_cast<uint16_t>(ClampIR(mac3))));
}

void GPF(GTE& gte, uint32_t opcode) {
    int32_t ir0 = static_cast<int32_t>(gte.ReadData(8));
    int32_t ir1 = static_cast<int16_t>(gte.ReadData(9) & 0xFFFF);
    int32_t ir2 = static_cast<int16_t>(gte.ReadData(10) & 0xFFFF);
    int32_t ir3 = static_cast<int16_t>(gte.ReadData(11) & 0xFFFF);

    int64_t mac1 = FixMul(ir1, ir0) >> 12;
    int64_t mac2 = FixMul(ir2, ir0) >> 12;
    int64_t mac3 = FixMul(ir3, ir0) >> 12;

    gte.WriteData(25, static_cast<uint32_t>(mac1));
    gte.WriteData(26, static_cast<uint32_t>(mac2));
    gte.WriteData(27, static_cast<uint32_t>(mac3));

    gte.WriteData(22, static_cast<uint32_t>(static_cast<uint16_t>(ClampIR(mac1))));
    gte.WriteData(23, static_cast<uint32_t>(static_cast<uint16_t>(ClampIR(mac2))));
    gte.WriteData(24, static_cast<uint32_t>(static_cast<uint16_t>(ClampIR(mac3))));
}

void GPL(GTE& gte, uint32_t opcode) {
    int32_t ir0 = static_cast<int32_t>(gte.ReadData(8));
    int32_t ir1 = static_cast<int16_t>(gte.ReadData(9) & 0xFFFF);
    int32_t ir2 = static_cast<int16_t>(gte.ReadData(10) & 0xFFFF);
    int32_t ir3 = static_cast<int16_t>(gte.ReadData(11) & 0xFFFF);

    int64_t mac1 = (FixMul(ir1, ir0) + (static_cast<int64_t>(ir1) << 12)) >> 12;
    int64_t mac2 = (FixMul(ir2, ir0) + (static_cast<int64_t>(ir2) << 12)) >> 12;
    int64_t mac3 = (FixMul(ir3, ir0) + (static_cast<int64_t>(ir3) << 12)) >> 12;

    gte.WriteData(25, static_cast<uint32_t>(mac1));
    gte.WriteData(26, static_cast<uint32_t>(mac2));
    gte.WriteData(27, static_cast<uint32_t>(mac3));

    gte.WriteData(22, static_cast<uint32_t>(static_cast<uint16_t>(ClampIR(mac1))));
    gte.WriteData(23, static_cast<uint32_t>(static_cast<uint16_t>(ClampIR(mac2))));
    gte.WriteData(24, static_cast<uint32_t>(static_cast<uint16_t>(ClampIR(mac3))));
}

}