#pragma once
#include <string>
#ifdef _WIN64
#include <Windows.h>
#elif __linux__
#include <unistd.h>
#endif

namespace Emulator {
/**
 * Universal file manipulation platform. Access file data on host machine.
 */
class File {
private:
    size_t m_sourceHandle;

public:
    explicit File(const char *sourcePath) {
        #ifdef _WIN64
        m_sourceHandle = (size_t)CreateFile(sourcePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        #elif __linux__
        m_sourceHandle = (size_t)open(sourcePath, O_RDONLY);
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
        std::string combinedPath = cwd + "\\" + suffixPath;
        return combinedPath;
    }

    /**
     * Access bytes from the file. This function is stream based, data could be extremely massive and could
     * damage the operating state of the host. The manufactured processor is expected to run near few TB of ram.
     * @
     */
};
}
