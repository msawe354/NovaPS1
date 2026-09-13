#include "Interrupts.hpp"

namespace nova::ps1 {

Interrupts::Interrupts() : _status(0), _mask(0), _pending(0) {
}

void Interrupts::Reset() {
    _status = 0;
    _mask = 0;
    _pending = 0;
}

void Interrupts::Request(uint32_t line) {
    if (line >= 32) return;
    _pending |= (1u << line);
    _status |= (1u << line);
}

void Interrupts::Clear(uint32_t line) {
    if (line >= 32) return;
    _pending &= ~(1u << line);
    _status &= ~(1u << line);
}

bool Interrupts::IsPending(uint32_t line) const {
    if (line >= 32) return false;
    return (_pending & (1u << line)) != 0;
}

bool Interrupts::HasPending() const {
    return (_pending & _mask) != 0;
}

void Interrupts::WriteStatus(uint32_t value) {
    _status = value & 0x7FF;
}

void Interrupts::WriteMask(uint32_t value) {
    _mask = value & 0x7FF;
}

}