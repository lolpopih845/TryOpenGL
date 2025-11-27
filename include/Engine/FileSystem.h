#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <filesystem>
#include <cstdlib> // for getenv
namespace Engine {
    class FileSystem {
    private:
        typedef std::string (*Builder)(const std::string& path);

    public:
        static std::string getPath(const std::string& path);

    private:
        // Return the root path from env or fallback
        static const std::string& getRoot();

        static Builder getPathBuilder();

        // Resolve relative to root
        static std::string getPathRelativeRoot(const std::string& path);

        // Resolve relative to executable directory
        static std::string getPathRelativeBinary(const std::string& path);

        // Provide a fallback if no root is defined
        inline static const char* logl_root = nullptr; // set this if needed
    };

} // Engine
#endif