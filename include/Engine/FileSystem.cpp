//
// Created by User1 on 27/11/2025.
//

#include "FileSystem.h"

namespace Engine {
    std::string FileSystem::getPath(const std::string &path)  {
        static Builder pathBuilder = getPathBuilder();
        return (*pathBuilder)(path);
    }

    const std::string & FileSystem::getRoot() {
        static const char* envRoot = std::getenv("LOGL_ROOT_PATH");
        static const char* givenRoot = (envRoot != nullptr ? envRoot : logl_root);
        static std::string root = (givenRoot != nullptr ? givenRoot : "");
        return root;
    }

    FileSystem::Builder FileSystem::getPathBuilder() {
        if (!getRoot().empty())
            return &FileSystem::getPathRelativeRoot;
        else
            return &FileSystem::getPathRelativeBinary;
    }

    std::string FileSystem::getPathRelativeRoot(const std::string &path) {
        std::filesystem::path fullPath = std::filesystem::path(getRoot()) / path;
        return std::filesystem::absolute(fullPath).string();
    }

    std::string FileSystem::getPathRelativeBinary(const std::string &path) {
        std::filesystem::path exeDir = std::filesystem::current_path(); // or use argv[0] path if available
        std::filesystem::path fullPath = exeDir / path;
        return std::filesystem::absolute(fullPath).string();
    }
} // Engine