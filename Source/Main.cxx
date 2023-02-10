#include "Memory/MemoryChip.hxx"
#include "Common/PacketBinary.hxx"

int main() {
    std::cout << "Hello World!" << std::endl;
    bool packet[] = { 0, 1, 0, 1 };
    std::cout << Emulator::PacketBinary::packetToDec(packet, 4) << std::endl;
    bool packetA[] = { 1, 1, 1, 1 };
    bool packetB[] = { 1, 1, 1, 1 };
    bool *packetC = Emulator::PacketBinary::addBinary(packetA, 4, packetB, 4, 5);
    for (size_t i = 0; i < 5; i++)
        std::cout << packetC[i];
    std::cout << std::endl;
    // print as dec
    std::cout << Emulator::PacketBinary::packetToDec(packetC, 5) << std::endl;
    return 0;
}