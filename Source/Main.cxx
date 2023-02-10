#include "Memory/MemoryChip.hxx"
#include "Common/PacketBinary.hxx"

int main() {
    std::cout << "Hello World!" << std::endl;
    bool *dec = Emulator::PacketBinary::decToPacket(255, 8);
    for (size_t i = 0; i < 8; i++)
        std::cout << dec[i];
    std::cout << std::endl;
    return 0;
}