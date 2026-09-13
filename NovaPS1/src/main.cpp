#include "Core/Emulator.hpp"
#include <iostream>
#include <string>

static void PrintBanner() {
    std::cout << "╔══════════════════════════════════╗" << std::endl;
    std::cout << "║        Nova-PS1 Emulator         ║" << std::endl;
    std::cout << "║       PlayStation 1 (C++)        ║" << std::endl;
    std::cout << "╚══════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
}

static void PrintUsage(const char* program) {
    std::cout << "Usage: " << program << " <bios.bin> [game.iso|game.exe]" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  " << program << " scph1001.bin" << std::endl;
    std::cout << "  " << program << " scph1001.bin crash.exe" << std::endl;
    std::cout << "  " << program << " scph1001.bin tekken.iso" << std::endl;
}

int main(int argc, char* argv[]) {
    PrintBanner();

    if (argc < 2) {
        PrintUsage(argv[0]);
        return 1;
    }

    nova::ps1::Emulator emu;

    if (!emu.Initialize()) {
        std::cerr << "[main] Failed to initialize emulator" << std::endl;
        return 1;
    }

    std::string biosPath = argv[1];
    if (!emu.LoadBios(biosPath)) {
        std::cerr << "[main] Failed to load BIOS: " << biosPath << std::endl;
        return 1;
    }

    std::cout << "[main] BIOS loaded: " << biosPath << std::endl;

    if (argc >= 3) {
        std::string gamePath = argv[2];

        if (gamePath.find(".exe") != std::string::npos) {
            if (!emu.LoadExe(gamePath)) {
                std::cerr << "[main] Failed to load EXE: " << gamePath << std::endl;
                return 1;
            }
            std::cout << "[main] EXE loaded: " << gamePath << std::endl;
        } else {
            if (!emu.LoadDisk(gamePath)) {
                std::cerr << "[main] Failed to load disk: " << gamePath << std::endl;
                return 1;
            }
            std::cout << "[main] Disk loaded: " << gamePath << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << "[main] Starting emulation..." << std::endl;
    std::cout << "[main] Press ESC to exit (when GUI is available)" << std::endl;
    std::cout << std::endl;

    emu.Reset();
    emu.Run();

    std::cout << std::endl;
    std::cout << "[main] Emulation stopped." << std::endl;
    std::cout << "[main] Total cycles: " << emu.GetCycleCount() << std::endl;

    return 0;
}