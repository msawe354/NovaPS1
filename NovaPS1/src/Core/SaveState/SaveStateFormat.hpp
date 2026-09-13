#pragma once
#include <cstdint>

namespace nova::ps1 {

struct SaveStateHeader {
    uint32_t magic;
    uint32_t version;
    uint32_t timestamp;
    uint32_t reserved;

    SaveStateHeader()
        : magic(0x4E4F5641),
          version(1),
          timestamp(0),
          reserved(0) {}
};

struct SaveStateSection {
    uint32_t tag;
    uint32_t size;

    SaveStateSection() : tag(0), size(0) {}
    SaveStateSection(uint32_t t, uint32_t s) : tag(t), size(s) {}
};

static constexpr uint32_t SECTION_CPU    = 0x43505500;
static constexpr uint32_t SECTION_GPU    = 0x47505500;
static constexpr uint32_t SECTION_SPU    = 0x53505500;
static constexpr uint32_t SECTION_DMA    = 0x444D4100;
static constexpr uint32_t SECTION_TIMERS = 0x54494D00;
static constexpr uint32_t SECTION_CDROM  = 0x43445200;
static constexpr uint32_t SECTION_PAD    = 0x50414400;
static constexpr uint32_t SECTION_END    = 0x454E4400;

}