#pragma once
#include <string>
#include <functional>
#ifdef _WIN64
#include <Windows.h>
#elif __linux__
#include <unistd.h>
#include <fcntl.h>
#include <linux/limits.h>
#endif

namespace Emulator {
/**
 * Universal file manipulation platform. Access file data on host machine.
 */
class File {
private:
    size_t m_sourceHandle;

public:
    explicit File(const std::string &sourcePath) {
        #ifdef _WIN64
        m_sourceHandle = (size_t)CreateFile(sourcePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        #elif __linux__
        m_sourceHandle = (size_t)open(sourcePath.c_str(), O_RDONLY);
        #endif
    }

    static std::string accessCWD() {
        #ifdef _WIN64
        char *cwd = new char[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, cwd);
        #elif __linux__
        char *cwd = new char[PATH_MAX];
        getcwd(cwd, PATH_MAX);
        #endif
        std::string cwdStr(cwd);
        delete[] cwd;
        return cwdStr;
    }

    static std::string combineCWDPath(const std::string &suffixPath) {
        std::string cwd = accessCWD();
        std::string combinedPath = cwd + "/" + suffixPath;
        return combinedPath;
    }

    void flipBinary(bool *binary, size_t binarySize) {
        bool *flippedBinary = new bool[binarySize];
        for (size_t i = 0; i < binarySize; i++)
            flippedBinary[i] = binary[binarySize - i - 1];
        for (size_t i = 0; i < binarySize; i++)
            binary[i] = flippedBinary[i];
        delete[] flippedBinary;
    }

    /**
     * Access bytes from the file. This function is stream based, data could be extremely massive and could
     * damage the operating state of the host. The manufactured processor is expected to run near few TB of ram.
     * @param byteCallback The callback function to access the bytes.
     */
    void readBytes(std::function<void(bool *byte)> byteCallback) {
        // Read the file in 8 bit chunks, if the chunk is as an 8 bit number, convert it to binary and pass it to the callback.
        #ifdef _WIN64
        DWORD bytesRead;
        char *byte = new char[1];
        while (ReadFile((HANDLE)m_sourceHandle, byte, 1, &bytesRead, NULL) && bytesRead > 0) {
            bool *byteBin = new bool[8];
            for (size_t i = 0; i < 8; i++)
                byteBin[i] = (byte[0] >> i) & 1;
            flipBinary(byteBin, 8);
            byteCallback(byteBin);
            delete[] byteBin;
        }
        delete[] byte;
        #elif __linux__
        char *byte = new char[1];
        while (read(m_sourceHandle, byte, 1) > 0) {
            bool *byteBin = new bool[8];
            for (size_t i = 0; i < 8; i++)
                byteBin[i] = (byte[0] >> i) & 1;
            flipBinary(byteBin, 8);
            byteCallback(byteBin);
            delete[] byteBin;
        }
        delete[] byte;
        #endif
    }
};
}
