#include "MIPS.hpp"
#include "Arithmetic/Arithmetic.hpp"
#include "Logical/Logical.hpp"
#include "Branch/Branch.hpp"
#include "Shift/Shift.hpp"
#include "LoadStore/LoadStore.hpp"
#include "Special/Special.hpp"
#include "FPU/FPU.hpp"
#include <iostream>

namespace nova::ps1 {

MIPS::MIPS(Bus& bus)
    : _bus(bus), _regs(), _tlb(), _icache(), _dcache(), _interrupts(),
      _delaySlotTarget(0), _inDelaySlot(false), _cycleCount(0) {
    InitTables();
}

void MIPS::Reset() {
    _regs.Reset();
    _tlb.Reset();
    _icache.Reset();
    _dcache.Reset();
    _interrupts.Reset();
    _delaySlotTarget = 0;
    _inDelaySlot = false;
    _cycleCount = 0;
}

void MIPS::InitTables() {
    _primaryTable.fill(&op_unknown);
    _specialTable.fill(&op_unknown);
    _regimmTable.fill(&op_unknown);
    _cop0Table.fill(&op_unknown);
    _cop1Table.fill(&op_unknown);
    _cop1SFuncTable.fill(&op_unknown);
    _cop2Table.fill(&op_unknown);

    _primaryTable[0x00] = &op_special;
    _primaryTable[0x01] = &op_regimm;
    _primaryTable[0x02] = &branch::J;
    _primaryTable[0x03] = &branch::JAL;
    _primaryTable[0x04] = &branch::BEQ;
    _primaryTable[0x05] = &branch::BNE;
    _primaryTable[0x06] = &branch::BLEZ;
    _primaryTable[0x07] = &branch::BGTZ;
    _primaryTable[0x08] = &arithmetic::ADDI;
    _primaryTable[0x09] = &arithmetic::ADDIU;
    _primaryTable[0x0A] = &arithmetic::SLTI;
    _primaryTable[0x0B] = &arithmetic::SLTIU;
    _primaryTable[0x0C] = &logical::ANDI;
    _primaryTable[0x0D] = &logical::ORI;
    _primaryTable[0x0E] = &logical::XORI;
    _primaryTable[0x0F] = &logical::LUI;
    _primaryTable[0x10] = &op_cop0;
    _primaryTable[0x11] = &op_cop1;
    _primaryTable[0x12] = &op_cop2;
    _primaryTable[0x20] = &loadstore::LB;
    _primaryTable[0x21] = &loadstore::LH;
    _primaryTable[0x22] = &loadstore::LWL;
    _primaryTable[0x23] = &loadstore::LW;
    _primaryTable[0x24] = &loadstore::LBU;
    _primaryTable[0x25] = &loadstore::LHU;
    _primaryTable[0x26] = &loadstore::LWR;
    _primaryTable[0x28] = &loadstore::SB;
    _primaryTable[0x29] = &loadstore::SH;
    _primaryTable[0x2A] = &loadstore::SWL;
    _primaryTable[0x2B] = &loadstore::SW;
    _primaryTable[0x2E] = &loadstore::SWR;
    _primaryTable[0x32] = &op_lwc2;
    _primaryTable[0x3A] = &op_swc2;

    _specialTable[0x00] = &shift::SLL;
    _specialTable[0x02] = &shift::SRL;
    _specialTable[0x03] = &shift::SRA;
    _specialTable[0x04] = &shift::SLLV;
    _specialTable[0x06] = &shift::SRLV;
    _specialTable[0x07] = &shift::SRAV;
    _specialTable[0x08] = &branch::JR;
    _specialTable[0x09] = &branch::JALR;
    _specialTable[0x0C] = &special::SYSCALL;
    _specialTable[0x0D] = &special::BREAK;
    _specialTable[0x10] = &arithmetic::MFHI;
    _specialTable[0x11] = &arithmetic::MTHI;
    _specialTable[0x12] = &arithmetic::MFLO;
    _specialTable[0x13] = &arithmetic::MTLO;
    _specialTable[0x18] = &arithmetic::MULT;
    _specialTable[0x19] = &arithmetic::MULTU;
    _specialTable[0x1A] = &arithmetic::DIV;
    _specialTable[0x1B] = &arithmetic::DIVU;
    _specialTable[0x20] = &arithmetic::ADD;
    _specialTable[0x21] = &arithmetic::ADDU;
    _specialTable[0x22] = &arithmetic::SUB;
    _specialTable[0x23] = &arithmetic::SUBU;
    _specialTable[0x24] = &logical::AND;
    _specialTable[0x25] = &logical::OR;
    _specialTable[0x26] = &logical::XOR;
    _specialTable[0x27] = &logical::NOR;
    _specialTable[0x2A] = &arithmetic::SLT;
    _specialTable[0x2B] = &arithmetic::SLTU;

    _regimmTable[0x00] = &branch::BLTZ;
    _regimmTable[0x01] = &branch::BGEZ;
    _regimmTable[0x10] = &branch::BLTZAL;
    _regimmTable[0x11] = &branch::BGEZAL;

    _cop1SFuncTable[0x00] = &fpu::ADD_S;
    _cop1SFuncTable[0x01] = &fpu::SUB_S;
    _cop1SFuncTable[0x02] = &fpu::MUL_S;
    _cop1SFuncTable[0x03] = &fpu::DIV_S;
    _cop1SFuncTable[0x04] = &fpu::SQRT_S;
    _cop1SFuncTable[0x05] = &fpu::ABS_S;
    _cop1SFuncTable[0x06] = &fpu::MOV_S;
    _cop1SFuncTable[0x07] = &fpu::NEG_S;
    _cop1SFuncTable[0x20] = &fpu::CVT_W_S;
    _cop1SFuncTable[0x21] = &fpu::CVT_W_S;
    _cop1SFuncTable[0x24] = &fpu::CVT_W_S;
    _cop1SFuncTable[0x30] = &fpu::C_EQ_S;
    _cop1SFuncTable[0x31] = &fpu::C_EQ_S;
    _cop1SFuncTable[0x32] = &fpu::C_EQ_S;
    _cop1SFuncTable[0x34] = &fpu::C_LT_S;
    _cop1SFuncTable[0x35] = &fpu::C_LT_S;
    _cop1SFuncTable[0x36] = &fpu::C_LT_S;
    _cop1SFuncTable[0x38] = &fpu::C_LE_S;
    _cop1SFuncTable[0x39] = &fpu::C_LE_S;
    _cop1SFuncTable[0x3A] = &fpu::C_LE_S;
}

void MIPS::Step() {
    uint32_t currentPc = _regs.GetPC();

    if (_interrupts.HasPending()) {
        HandleException(0);
        _cycleCount++;
        return;
    }

    uint32_t opcode = _bus.Read32(currentPc);

    _regs.SetNextPC(currentPc + 4);

    Execute(opcode);

    if (_inDelaySlot) {
        uint32_t delayPc = _regs.GetNextPC();
        uint32_t delayOpcode = _bus.Read32(delayPc);
        _regs.SetPC(delayPc);
        _regs.SetNextPC(delayPc + 4);

        uint32_t savedDelayTarget = _delaySlotTarget;
        _inDelaySlot = false;

        Execute(delayOpcode);

        _regs.SetPC(savedDelayTarget);
        _regs.SetNextPC(savedDelayTarget + 4);
        _inDelaySlot = false;
    } else {
        _regs.SetPC(_regs.GetNextPC());
    }

    _cycleCount++;
}

void MIPS::Execute(uint32_t opcode) {
    uint32_t primary = (opcode >> 26) & 0x3F;
    _primaryTable[primary](*this, opcode);
}

void MIPS::SetBranchTarget(uint32_t target) {
    _delaySlotTarget = target;
    _inDelaySlot = true;
}

void MIPS::HandleException(uint32_t cause) {
    uint32_t status = _regs.GetCOP0(12);
    uint32_t epc = _regs.GetPC();

    if (_inDelaySlot) {
        epc -= 4;
    }

    _regs.SetCOP0(13, cause << 2);
    _regs.SetCOP0(14, epc);

    uint32_t vector = (status & 0x00400000) ? 0x80000180 : 0xBFC00180;
    _regs.SetPC(vector);
    _regs.SetNextPC(vector + 4);
    _inDelaySlot = false;
}

void MIPS::op_unknown(uint32_t opcode) {
    std::cerr << "[MIPS] Unknown opcode: 0x" << std::hex << opcode
              << " at PC: 0x" << _regs.GetPC() << std::dec << std::endl;
    HandleException(10);
}

void MIPS::op_special(uint32_t opcode) {
    uint32_t func = Func(opcode);
    _specialTable[func](*this, opcode);
}

void MIPS::op_regimm(uint32_t opcode) {
    uint32_t rt = Rt(opcode);
    _regimmTable[rt](*this, opcode);
}

void MIPS::op_cop0(uint32_t opcode) {
    uint32_t rs = Rs(opcode);
    uint32_t rt = Rt(opcode);
    uint32_t rd = Rd(opcode);

    switch (rs) {
        case 0x00:
            _regs.SetGPR(rt, _regs.GetCOP0(rd));
            break;
        case 0x04:
            _regs.SetCOP0(rd, _regs.GetGPR(rt));
            break;
        case 0x10:
            break;
        default:
            op_unknown(opcode);
            break;
    }
}

void MIPS::op_cop1(uint32_t opcode) {
    uint32_t rs = Rs(opcode);
    uint32_t rt = Rt(opcode);
    uint32_t rd = Rd(opcode);

    switch (rs) {
        case 0x00:
            fpu::MFC1(*this, opcode);
            break;
        case 0x02:
            fpu::CFC1(*this, opcode);
            break;
        case 0x04:
            fpu::MTC1(*this, opcode);
            break;
        case 0x06:
            fpu::CTC1(*this, opcode);
            break;
        case 0x08:
        {
            uint32_t fmt = (opcode >> 21) & 0x1F;
            uint32_t func = Func(opcode);

            if (fmt == 0x10) {
                _cop1SFuncTable[func](*this, opcode);
            } else if (fmt == 0x11) {
                _cop1SFuncTable[func](*this, opcode);
            } else {
                op_unknown(opcode);
            }
            break;
        }
        default:
            op_unknown(opcode);
            break;
    }
}

void MIPS::op_cop2(uint32_t opcode) {
    uint32_t rs = Rs(opcode);
    uint32_t rt = Rt(opcode);
    uint32_t rd = Rd(opcode);

    switch (rs) {
        case 0x00:
            _regs.SetGPR(rt, _regs.GetCOP2(rd));
            break;
        case 0x02:
            _regs.SetGPR(rt, _regs.GetCOP2Control(rd));
            break;
        case 0x04:
            _regs.SetCOP2(rd, _regs.GetGPR(rt));
            break;
        case 0x06:
            _regs.SetCOP2Control(rd, _regs.GetGPR(rt));
            break;
        case 0x10:
            break;
        default:
            op_unknown(opcode);
            break;
    }
}

void MIPS::op_lwc2(uint32_t opcode) {
    uint32_t rs = Rs(opcode);
    uint32_t rt = Rt(opcode);
    uint32_t addr = _regs.GetGPR(rs) + static_cast<uint32_t>(Simm16(opcode));
    _regs.SetCOP2(rt, _bus.Read32(addr));
}

void MIPS::op_swc2(uint32_t opcode) {
    uint32_t rs = Rs(opcode);
    uint32_t rt = Rt(opcode);
    uint32_t addr = _regs.GetGPR(rs) + static_cast<uint32_t>(Simm16(opcode));
    _bus.Write32(addr, _regs.GetCOP2(rt));
}

}