#include "FPU.hpp"
#include "../MIPS.hpp"
#include "../Registers.hpp"
#include "../../Memory/Bus.hpp"
#include <cmath>
#include <cstring>

namespace nova::ps1::fpu {

static inline uint32_t Rs(uint32_t opcode) { return (opcode >> 21) & 0x1F; }
static inline uint32_t Rt(uint32_t opcode) { return (opcode >> 16) & 0x1F; }
static inline uint32_t Rd(uint32_t opcode) { return (opcode >> 11) & 0x1F; }
static inline int16_t Simm16(uint32_t opcode) { return static_cast<int16_t>(opcode & 0xFFFF); }
static inline uint32_t Func(uint32_t opcode) { return opcode & 0x3F; }

static inline uint32_t F2U(float value) {
    uint32_t result;
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

static inline float U2F(uint32_t value) {
    float result;
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

static inline uint64_t D2U(double value) {
    uint64_t result;
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

static inline double U2D(uint64_t value) {
    double result;
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

void ADD_S(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    float a = U2F(regs.GetCOP1(Rs(opcode)));
    float b = U2F(regs.GetCOP1(Rt(opcode)));
    regs.SetCOP1(Rd(opcode), F2U(a + b));
}

void ADD_D(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    double a = U2D(regs.GetCOP1D(Rs(opcode)));
    double b = U2D(regs.GetCOP1D(Rt(opcode)));
    regs.SetCOP1D(Rd(opcode), D2U(a + b));
}

void SUB_S(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    float a = U2F(regs.GetCOP1(Rs(opcode)));
    float b = U2F(regs.GetCOP1(Rt(opcode)));
    regs.SetCOP1(Rd(opcode), F2U(a - b));
}

void SUB_D(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    double a = U2D(regs.GetCOP1D(Rs(opcode)));
    double b = U2D(regs.GetCOP1D(Rt(opcode)));
    regs.SetCOP1D(Rd(opcode), D2U(a - b));
}

void MUL_S(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    float a = U2F(regs.GetCOP1(Rs(opcode)));
    float b = U2F(regs.GetCOP1(Rt(opcode)));
    regs.SetCOP1(Rd(opcode), F2U(a * b));
}

void MUL_D(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    double a = U2D(regs.GetCOP1D(Rs(opcode)));
    double b = U2D(regs.GetCOP1D(Rt(opcode)));
    regs.SetCOP1D(Rd(opcode), D2U(a * b));
}

void DIV_S(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    float a = U2F(regs.GetCOP1(Rs(opcode)));
    float b = U2F(regs.GetCOP1(Rt(opcode)));
    regs.SetCOP1(Rd(opcode), F2U(a / b));
}

void DIV_D(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    double a = U2D(regs.GetCOP1D(Rs(opcode)));
    double b = U2D(regs.GetCOP1D(Rt(opcode)));
    regs.SetCOP1D(Rd(opcode), D2U(a / b));
}

void SQRT_S(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    float a = U2F(regs.GetCOP1(Rs(opcode)));
    regs.SetCOP1(Rd(opcode), F2U(std::sqrt(a)));
}

void ABS_S(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    float a = U2F(regs.GetCOP1(Rs(opcode)));
    regs.SetCOP1(Rd(opcode), F2U(std::fabs(a)));
}

void MOV_S(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetCOP1(Rd(opcode), regs.GetCOP1(Rs(opcode)));
}

void NEG_S(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    float a = U2F(regs.GetCOP1(Rs(opcode)));
    regs.SetCOP1(Rd(opcode), F2U(-a));
}

void CVT_S_W(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    int32_t a = static_cast<int32_t>(regs.GetCOP1(Rs(opcode)));
    regs.SetCOP1(Rd(opcode), F2U(static_cast<float>(a)));
}

void CVT_W_S(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    float a = U2F(regs.GetCOP1(Rs(opcode)));
    regs.SetCOP1(Rd(opcode), static_cast<uint32_t>(static_cast<int32_t>(a)));
}

void CVT_S_D(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    double a = U2D(regs.GetCOP1D(Rs(opcode)));
    regs.SetCOP1(Rd(opcode), F2U(static_cast<float>(a)));
}

void CVT_D_S(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    float a = U2F(regs.GetCOP1(Rs(opcode)));
    regs.SetCOP1D(Rd(opcode), D2U(static_cast<double>(a)));
}

void CVT_W_D(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    double a = U2D(regs.GetCOP1D(Rs(opcode)));
    regs.SetCOP1(Rd(opcode), static_cast<uint32_t>(static_cast<int32_t>(a)));
}

void CVT_D_W(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    int32_t a = static_cast<int32_t>(regs.GetCOP1(Rs(opcode)));
    regs.SetCOP1D(Rd(opcode), D2U(static_cast<double>(a)));
}

void C_EQ_S(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    float a = U2F(regs.GetCOP1(Rs(opcode)));
    float b = U2F(regs.GetCOP1(Rt(opcode)));
    uint32_t fcr31 = regs.GetFCR31();
    if (a == b) fcr31 |= 0x800000;
    else fcr31 &= ~0x800000;
    regs.SetFCR31(fcr31);
}

void C_LT_S(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    float a = U2F(regs.GetCOP1(Rs(opcode)));
    float b = U2F(regs.GetCOP1(Rt(opcode)));
    uint32_t fcr31 = regs.GetFCR31();
    if (a < b) fcr31 |= 0x800000;
    else fcr31 &= ~0x800000;
    regs.SetFCR31(fcr31);
}

void C_LE_S(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    float a = U2F(regs.GetCOP1(Rs(opcode)));
    float b = U2F(regs.GetCOP1(Rt(opcode)));
    uint32_t fcr31 = regs.GetFCR31();
    if (a <= b) fcr31 |= 0x800000;
    else fcr31 &= ~0x800000;
    regs.SetFCR31(fcr31);
}

void C_EQ_D(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    double a = U2D(regs.GetCOP1D(Rs(opcode)));
    double b = U2D(regs.GetCOP1D(Rt(opcode)));
    uint32_t fcr31 = regs.GetFCR31();
    if (a == b) fcr31 |= 0x800000;
    else fcr31 &= ~0x800000;
    regs.SetFCR31(fcr31);
}

void C_LT_D(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    double a = U2D(regs.GetCOP1D(Rs(opcode)));
    double b = U2D(regs.GetCOP1D(Rt(opcode)));
    uint32_t fcr31 = regs.GetFCR31();
    if (a < b) fcr31 |= 0x800000;
    else fcr31 &= ~0x800000;
    regs.SetFCR31(fcr31);
}

void C_LE_D(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    double a = U2D(regs.GetCOP1D(Rs(opcode)));
    double b = U2D(regs.GetCOP1D(Rt(opcode)));
    uint32_t fcr31 = regs.GetFCR31();
    if (a <= b) fcr31 |= 0x800000;
    else fcr31 &= ~0x800000;
    regs.SetFCR31(fcr31);
}

void MFC1(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rt(opcode), regs.GetCOP1(Rd(opcode)));
}

void MTC1(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetCOP1(Rd(opcode), regs.GetGPR(Rt(opcode)));
}

void CFC1(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetGPR(Rt(opcode), regs.GetFCR31());
}

void CTC1(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    regs.SetFCR31(regs.GetGPR(Rt(opcode)));
}

void LWC1(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t addr = regs.GetGPR(Rs(opcode)) + static_cast<uint32_t>(Simm16(opcode));
    regs.SetCOP1(Rt(opcode), cpu.GetBus().Read32(addr));
}

void SWC1(MIPS& cpu, uint32_t opcode) {
    Registers& regs = cpu.GetRegisters();
    uint32_t addr = regs.GetGPR(Rs(opcode)) + static_cast<uint32_t>(Simm16(opcode));
    cpu.GetBus().Write32(addr, regs.GetCOP1(Rt(opcode)));
}

}