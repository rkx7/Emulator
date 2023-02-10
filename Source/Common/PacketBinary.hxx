#pragma once
#include <iostream>
#include <exception>

namespace Emulator {
/**
 * Flags object, this can be passed to PacketBinary for manipulation and see effective
 * flags.
 */
struct ALUFlags {
    /**
     * This flag is set when the result of the operation is negative.
     */
    bool negative = false;

    /**
     * This flag is set when the result of the operation is zero.
     */
    bool zero = false;

    /**
     * This flag is set when the result of the operation is positive.
     */
    bool positive = false;

    /**
     * This flag is set when the result of the operation is overflow.
     */
    bool overflow = false;
};

/**
 * This is a class that manipulates binary data in packets. It can add values, subtract, and 
 * manipulate the binary data in packets. It can manipulate data beyond the bit capabilities of
 * the processor. Larger bits of binary must be on the left, smaller bits on the right. Any new bits that are added must
 * be added to the left to ensure the value of the number does not get influenced by the new bits.
 */ 
class PacketBinary {
public:
    /**
     * This packet to decimal converter can convert binary numbers up to 64 bits.
     * @param packet The packet to convert to decimal.
     * @param packetSize The size of the packet, or bit width of the packet.
     */
    static size_t packetToDec(bool *packet, size_t packetSize) {
        if (packetSize > 64)
            throw std::runtime_error("Packet size is too large. Packet size must be less than or equal to 64.");
        bool *packetBigEndian = new bool[packetSize];
        for (size_t i = 0; i < packetSize; i++)
            packetBigEndian[i] = packet[packetSize - i - 1];
        size_t dec = 0;
        for (size_t i = 0; i < packetSize; i++)
            dec += packetBigEndian[i] * (1 << i);
        return dec;
    }

    /**
     * This binary adder adds numbers without the 64 bit limit. The 64 limit exists on the fast conversion due 
     * to the address bit width. This one does not write any decimal values to 64 bit variables. So this function
     * can handle binary numbers with more than 64 bit numbers.
     * @param packetA The first packet to add.
     * @param packetASize The size of the first packet.
     * @param packetB The second packet to add.
     * @param packetBSize The size of the second packet.
     * @param outputPacketSize The size of the output packet or the size of the largest packet.
     * @param flags The flags object to set the flags on.
     * @return The output packet.
     */
    static bool *addBinary(bool *packetA, size_t packetASize, bool *packetB, size_t packetBSize, size_t outputPacketSize, ALUFlags *flags) {
        size_t cary = 0;
        bool *packetC = new bool[outputPacketSize];

        if (packetASize > outputPacketSize || packetBSize > outputPacketSize)
            throw std::runtime_error("Packet size is too large. Packet size must be less than or equal to 64.");

        if (packetASize < outputPacketSize) {
            bool *packetA2 = new bool[outputPacketSize];
            for (size_t i = 0; i < outputPacketSize; i++)
                packetA2[i] = 0;
            for (size_t i = 0; i < packetASize; i++)
                packetA2[outputPacketSize - i - 1] = packetA[packetASize - i - 1];
            packetA = packetA2;
        }

        if (packetBSize < outputPacketSize) {
            bool *packetB2 = new bool[outputPacketSize];
            for (size_t i = 0; i < outputPacketSize; i++)
                packetB2[i] = 0;
            for (size_t i = 0; i < packetBSize; i++)
                packetB2[outputPacketSize - i - 1] = packetB[packetBSize - i - 1];
            packetB = packetB2;
        }

        for (size_t ir = outputPacketSize; ir > 0; ir--) {
            size_t i = ir - 1;
            
            size_t sum = packetA[i] + packetB[i] + cary;

            if (sum == 0) {
                packetC[i] = 0;
                cary = 0;
            } else if (sum == 1) {
                packetC[i] = 1;
                cary = 0;
            } else if (sum == 2) {
                packetC[i] = 0;
                cary = 1;
            } else if (sum == 3) {
                packetC[i] = 1;
                cary = 1;
            }
        }

        if (cary > 0)
            flags->overflow = true;
        bool allZero = true;
        for (size_t i = 0; i < outputPacketSize; i++)
            if (packetC[i] == 1) allZero = false;
        if (allZero) flags->zero = true;
        else flags->positive = true;

        return packetC;
    }

    /**
     * This function converts a decimal number to a binary packet.
     * @param dec The decimal number to convert.
     * @param packetSize The size of the packet.
     * @return The binary packet.
     */
    static bool *decToPacket(size_t dec, size_t packetSize) {
        if (packetSize > 64)
            throw std::runtime_error("Packet size is too large. Packet size must be less than or equal to 64.");
        bool *packet = new bool[packetSize];
        for (size_t i = 0; i < packetSize; i++)
            packet[i] = 0;
        for (size_t i = 0; i < packetSize; i++) {
            if (dec % 2 == 1)
                packet[packetSize - i - 1] = 1;
            dec /= 2;
        }
        return packet;
    }

    /**
     * Compare two binary packets. See if packet A is larger than packet B.
     * @param packetA The first packet to compare.
     * @param packetASize The size of the first packet.
     * @param packetB The second packet to compare.
     * @param packetBSize The size of the second packet.
     * @param flags The flags object to set the flags on.
     * @return Whether packet A is larger than packet B is.
     */
    static bool compareBinarySize(bool *packetA, size_t packetASize, bool *packetB, size_t packetBSize, ALUFlags *flags) {
        for (size_t i = 0; i < packetASize; i++)
            if (packetA[i] == 1 && packetB[i] == 0) {
                flags->positive = true;
                return true;
            } else if (packetA[i] == 0 && packetB[i] == 1) {
                flags->negative = true;
                return false;
            }

        flags->zero = true;
        return false;
    }

    /**
     * This binary subtractor subtracts numbers without the 64 bit limit. The 64 limit exists on the fast conversion due
     * to the address bit width. This one does not write any decimal values to 64 bit variables. So this function
     * can handle binary numbers with more than 64 bit numbers.
     * @param packetA The first packet to subtract.
     * @param packetASize The size of the first packet.
     * @param packetB The second packet to subtract.
     * @param packetBSize The size of the second packet.
     * @param outputPacketSize The size of the output packet or the size of the largest packet.
     * @param flags The flags object to set the flags on.
     * @return The output packet.
     */
    static bool *subtractBinary(bool *packetA, size_t packetASize, bool *packetB, size_t packetBSize, size_t outputPacketSize, ALUFlags *flags) {
        size_t cary = 0;        
        bool *packetC = new bool[outputPacketSize];

        if (packetASize > outputPacketSize || packetBSize > outputPacketSize)
            throw std::runtime_error("Packet size is too large. Packet size must be less than or equal to 64.");

        if (packetASize < outputPacketSize) {
            bool *packetA2 = new bool[outputPacketSize];
            for (size_t i = 0; i < outputPacketSize; i++)
                packetA2[i] = 0;
            for (size_t i = 0; i < packetASize; i++)
                packetA2[outputPacketSize - i - 1] = packetA[packetASize - i - 1];
            packetA = packetA2;
        }

        if (packetBSize < outputPacketSize) {
            bool *packetB2 = new bool[outputPacketSize];
            for (size_t i = 0; i < outputPacketSize; i++)
                packetB2[i] = 0;
            for (size_t i = 0; i < packetBSize; i++)
                packetB2[outputPacketSize - i - 1] = packetB[packetBSize - i - 1];
            packetB = packetB2;
        }

        for (size_t ir = outputPacketSize; ir > 0; ir--) {
            size_t i = ir - 1;

            size_t sum = packetA[i] - packetB[i] - cary;

            if (sum == 0) {
                packetC[i] = 0;
                cary = 0;
            } else if (sum == 1) {
                packetC[i] = 1;
                cary = 0;
            } else if (sum == 2) {
                packetC[i] = 0;
                cary = 1;
            } else if (sum == 3) {
                packetC[i] = 1;
                cary = 1;
            }
        }

        if (cary > 0)
            flags->overflow = true;
        bool allZero = true;
        for (size_t i = 0; i < outputPacketSize; i++)
            if (packetC[i] == 1) allZero = false;
        if (allZero) flags->zero = true;

        bool largerPacket = compareBinarySize(packetA, packetASize, packetB, packetBSize, flags);
        if (largerPacket) flags->positive = true;
        else flags->negative = true;
        return packetC;
    }
};
}