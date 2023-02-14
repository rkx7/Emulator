#include "Memory/MemoryChip.hxx"
#include "Common/PacketBinary.hxx"
#include "Platform/Host/File.hxx"

using namespace Emulator;

int main() {
    File file(File::combineCWDPath("../abxyz"));
    std::cout << File::combineCWDPath("abxyz") << std::endl;

    file.readBytes([](bool *byte) {
        for (size_t i = 0; i < 8; i++)
            std::cout << byte[i];
        std::cout << std::endl;
    });
    return 0;
}