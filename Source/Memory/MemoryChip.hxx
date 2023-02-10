#pragma once
#include <iostream>

namespace Emulator {
/**
 * High speed memory chip emulator that has support for multi-bit platforms, such as
 * 8 bit computing, 16 bit computing, 64 bit computing and more. This memory chip is asyncronous. 
 */
class MemoryChip {
private:
    /**
     * This is a default initialized memory packet. It should contain the default binary
     * data that the memory device should be started up with.
     */
    bool *m_memoryEmptyPacket;

    /**
     * The highest address that the memory device can address to. 
     */
    bool *m_topAddresser;

public:
    /**
     * @param memoryEmptyPacket The default initialized memory packet. It is highly 
     * recommended that this is a pointer to an array filled with (x) amount of 0's. The (x)
     * represents the value width of the memory chip. The number of elements in this array
     * will also determine what the peak addresser will be. 
     * @param topAddresser The highest address that the memory device can address to. The value of 
     * this parameter should be either equal to or less than the number of elements in the
     * memoryEmptyPacket array.  
     */
    MemoryChip(bool *memoryEmptyPacket, bool *topAddresser) 
        : m_memoryEmptyPacket(memoryEmptyPacket), m_topAddresser(topAddresser) {
        
    }
};
}