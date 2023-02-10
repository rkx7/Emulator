#pragma once
#include <iostream>
#include <exception>

namespace Emulator {
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
     */
    static bool *addBinary(bool *packetA, size_t packetASize, bool *packetB, size_t packetBSize, size_t outputPacketSize) {
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

        return packetC;
    }

    /**
     * This binary subtractor subtracts numbers without the 64 bit limit. The 64 limit exists on the fast conversion due
     * to the address bit width. This one does not write any decimal values to 64 bit variables. So this function
     * can handle binary numbers with more than 64 bit numbers.
     */
    static bool *subtractBinary(bool *packetA, size_t packetASize, bool *packetB, size_t packetBSize, size_t outputPacketSize) {
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

        return packetC;
    }
};
}