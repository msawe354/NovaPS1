#pragma once
#include "Registers.hpp"
#include "TLB/TLB.hpp"
#include "Cache/Cache.hpp"
#include "Interrupts/Interrupts.hpp"
#include "../Memory/Bus.hpp"
#include <cstdint>
#include <array>

namespace nova::ps1 {

class MIPS {
public:
    MIPS(Bus& bus);
    ~MIPS() = default;

    void Reset();
    void Step();

    Registers& GetRegisters() { return _regs; }
    const Registers& GetRegisters() const { return _regs; }
    Bus& GetBus() { return _bus; }
    TLB& GetTLB() { return _tlb; }
    Cache& GetICache() { return _icache; }
    Cache& GetDCache() { return _dcache; }
    Interrupts& GetInterrupts() { return _interrupts; }

    uint32_t GetPC() const { return _regs.GetPC(); }
    uint64_t GetCycleCount() const { return _cycleCount; }

    void SetBranchTarget(uint32_t target);
    bool IsInDelaySlot() const { return _inDelaySlot; }
    void HandleException(uint32_t cause);

private:
    using OpcodeHandler = void (*)(MIPS&, uint32_t);

    Bus& _bus;
    Registers _regs;
    TLB _tlb;
    Cache _icache;
    Cache _dcache;
    Interrupts _interrupts;

    std::array<OpcodeHandler, 64> _primaryTable;
    std::array<OpcodeHandler, 64> _specialTable;
    std::array<OpcodeHandler, 64> _regimmTable;
    std::array<OpcodeHandler, 64> _cop0Table;
    std::array<OpcodeHandler, 64> _cop1Table;
    std::array<OpcodeHandler, 64> _cop1SFuncTable;
    std::array<OpcodeHandler, 64> _cop2Table;

    uint32_t _delaySlotTarget;
    bool _inDelaySlot;
    uint64_t _cycleCount;

    void InitTables();
    void Execute(uint32_t opcode);

    uint32_t Rs(uint32_t opcode) const { return (opcode >> 21) & 0x1F; }
    uint32_t Rt(uint32_t opcode) const { return (opcode >> 16) & 0x1F; }
    uint32_t Rd(uint32_t opcode) const { return (opcode >> 11) & 0x1F; }
    uint32_t Shamt(uint32_t opcode) const { return (opcode >> 6) & 0x1F; }
    uint32_t Func(uint32_t opcode) const { return opcode & 0x3F; }
    uint32_t Imm16(uint32_t opcode) const { return opcode & 0xFFFF; }
    int16_t Simm16(uint32_t opcode) const { return static_cast<int16_t>(opcode & 0xFFFF); }
    uint32_t Target(uint32_t opcode) const { return (opcode & 0x03FFFFFF) << 2; }

    void op_unknown(uint32_t opcode);
    void op_special(uint32_t opcode);
    void op_regimm(uint32_t opcode);
    void op_cop0(uint32_t opcode);
    void op_cop1(uint32_t opcode);
    void op_cop2(uint32_t opcode);
    void op_lwc2(uint32_t opcode);
    void op_swc2(uint32_t opcode);
};

}