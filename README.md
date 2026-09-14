Nova-PS1

Nova-PS1 is a Sony PlayStation 1 emulator written from scratch in C++20. The project focuses on low-level emulation: MIPS R3000A, GTE, GPU, SPU, DMA, Timers, CD-ROM, BIOS HLE, and Pad.

Table of Contents

    Overview

    Features

    Architecture

    Project Structure

    Requirements

    Build

    Usage

    Controls

    Save States

    Tech Stack

    Roadmap

    License

Overview

Nova-PS1 is an educational project aimed at understanding how the PlayStation 1 works at every level: from MIPS instructions to GTE 3D geometry and GPU rendering. The emulator is written from scratch without any third-party emulation libraries.

The project is under active development. Core subsystems (CPU, GPU, SPU, DMA, Timers, CD-ROM, BIOS HLE, Pad, Save States) are implemented.
Features
CPU (MIPS R3000A)

    Full MIPS R3000A interpreter

    All core instructions (Arithmetic, Logical, Branch, Load/Store, Shift, Special)

    Delay Slot with correct handling

    COP0 (control, exceptions, TLB)

    COP1 (FPU: ADD.S, SUB.S, MUL.S, DIV.S, SQRT.S, C.EQ.S, C.LT.S, C.LE.S)

    COP2 (GTE)

    Instruction Cache and Data Cache

    Interrupts (VBlank, GPU, CDROM, DMA, Timer0-2, Controller, SPU, PIO)

GTE (Geometry Transformation Engine)

    RTPS / RTPT (3D vertex transformation)

    NCS / NCT / NCDS / NCDT (lighting)

    NCCS / NCCT / CC / CDP (color)

    DPCS / DPCT / DCPL / INTPL (color interpolation)

    DPCL / DPST (depth)

    AVSZ3 / AVSZ4 (Z averaging)

    NCLIP (clipping)

    MVMVA / OP / SQR / GPF / GPL (vector operations)

GPU

    VRAM 1024x512 (2 MB)

    GP0 (rendering commands)

    GP1 (control)

    Rasterizer (triangles, rectangles, lines)

    Flat and Gouraud shading

    Texture (4-bit, 8-bit, 15-bit)

    Texture Cache

    Texture Window

    CLUT (Color Look-Up Table)

    Blending (Half, Add, Subtract, AddQuarter)

    Texture Blend (Modulate, Decal, Highlight, Highlight2)

    Drawing Area (Scissor)

SPU

    24 voices

    ADPCM decoding

    ADSR envelope

    Main Volume

    Reverb (basic)

    Sample Buffer (4096 samples)

DMA

    7 channels (MDEC In, MDEC Out, GPU, CDROM, SPU, PIO, OTC)

    Block Transfer

    Linked List Transfer

    Sync Mode 0/1/2

    Interrupt

Timers

    3 timers (Timer0, Timer1, Timer2)

    Clock Source (System, DotClock, HBlank, System/8)

    Target / Overflow IRQ

    Repeat / Pulse mode

CD-ROM

    ISO / BIN / CUE loading

    Commands: GetStat, GetID, SetLoc, Seek, ReadN, ReadS, Pause, Init, GetlocL, GetlocP, Setmode, Getmode, Test

    Parameter FIFO

    Response FIFO

    IRQ

BIOS HLE

    Syscall A0 (PutChar, PutString, LoadExe, FlushCache, Event functions)

    Syscall B0 (stubs)

    Syscall C0 (stubs)

    Kernel Table

    Syscall Table

Pad

    2 ports

    All buttons (Select, L3, R3, Start, D-Pad, L1/R1, L2/R2, Triangle, Circle, Cross, Square)

    Key Mapper

    IRQ

Save States

    Serialization of CPU / GPU / SPU / DMA / Timers / CDROM / Pad

    Magic + Version

    Validator

    Compressor (RLE)

    Save Manager (10 slots)

Emulator

    Main Loop (CPU + Sync Components)

    Load BIOS

    Load Disk (ISO/BIN/CUE)

    Load EXE (PS-X EXE)

    Framebuffer Access

    Key Input

Architecture
text

+---------------------------------------------+
|                 Emulator                    |
|                                             |
|  +----------+    +----------+    +-------+  |
|  |  MIPS    |<-->|   Bus    |<-->|  GPU  |  |
|  | (CPU)    |    | (Memory) |    |(VRAM) |  |
|  +----+-----+    +----+-----+    +-------+  |
|       |               |                     |
|  +----v-----+    +----v-----+    +-------+  |
|  |   GTE    |    |   DMA    |    |  SPU  |  |
|  +----------+    +----------+    +-------+  |
|                                             |
|  +----------+    +----------+    +-------+  |
|  | Timers   |    | CDROM    |    |  Pad  |  |
|  +----------+    +----------+    +-------+  |
|                                             |
|  +----------+    +----------+               |
|  |  BIOS    |    |SaveState |               |
|  |  (HLE)   |    |          |               |
|  +----------+    +----------+               |
+---------------------------------------------+

Project Structure
text

Nova-PS1/
├── CMakeLists.txt
├── README.md
├── src/
│   ├── main.cpp
│   └── Core/
│       ├── Emulator.hpp
│       ├── Emulator.cpp
│       ├── Bus/
│       ├── CPU/
│       │   ├── MIPS.hpp
│       │   ├── MIPS.cpp
│       │   ├── Registers.hpp
│       │   ├── Registers.cpp
│       │   ├── Arithmetic/
│       │   ├── Logical/
│       │   ├── Branch/
│       │   ├── Shift/
│       │   ├── LoadStore/
│       │   ├── Special/
│       │   ├── FPU/
│       │   ├── GTE/
│       │   ├── TLB/
│       │   ├── Cache/
│       │   └── Interrupts/
│       ├── GPU/
│       ├── SPU/
│       ├── DMA/
│       ├── Timers/
│       ├── CDROM/
│       ├── BIOS/
│       ├── Pad/
│       └── SaveState/

Requirements

    Compiler: C++20 (MSVC 2019+, GCC 10+, Clang 11+)

    Build System: CMake 3.20+

    OS: Windows 10+, Linux, macOS 10.15+

    RAM: 512 MB+

    Storage: 50 MB

Build
bash

git clone https://github.com/your-username/Nova-PS1.git
cd Nova-PS1
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

Executable will be in build/bin/nova-ps1.
Windows (MSVC)
bash

cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release

Usage
bash

nova-ps1 <bios.bin> [game.exe|game.iso|game.bin|game.cue]

Examples
bash

nova-ps1 scph1001.bin
nova-ps1 scph1001.bin crash.exe
nova-ps1 scph1001.bin tekken.iso

Controls
Keyboard	PS1 Button
Arrow Keys	D-Pad
X	Cross
C	Circle
S	Square
D	Triangle
Q	L1
E	R1
A	L2
R	R2
Enter	Start
Shift	Select
Z	L3
V	R3
ESC	Exit
Save States

    10 slots

    Format: .state

    Path: saves/slot<N>.state

    Compressor (RLE) for size reduction

    Validator for integrity check

Tech Stack
Component	Technology
Language	C++20
Build	CMake
Platforms	Windows, Linux, macOS
Dependencies	None (currently)
Roadmap
Version	Features
v0.1	✅ CPU + GTE + Bus
v0.2	✅ GPU + SPU
v0.3	✅ DMA + Timers + CDROM
v0.4	✅ BIOS HLE + Pad
v0.5	✅ Save States
v1.0	🔜 GUI (SDL2)
v1.1	🔜 JIT (Dynamic Recompiler)
v1.2	🔜 Netplay
License

MIT License. See LICENSE for details.
