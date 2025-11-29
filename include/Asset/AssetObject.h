#ifndef ASSETOBJECT_H
#define ASSETOBJECT_H
#include <string>
#include <utility>

#include "../Engine/FileSystem.h"

namespace Asset {
    enum AssetType {
        TEXTURE,
        SHADER,
        MATERIAL,
        SKELETON,
        MODEL,
        SOUND,
        ANIMATION
    };
    class AssetObject {
    public:
        std::string name;
        std::string path;
        std::vector<std::string> paths;
        AssetType type;
        AssetObject(const std::string& name, const std::string& path, AssetType type)
    : name(name), paths{path}, type(type) {}

        // Multiple paths
        AssetObject(const std::string& name, std::vector<std::string> paths, AssetType type)
            : name(name), paths(std::move(paths)), type(type) {}
        virtual ~AssetObject() = default;
        std::string getAbsPath() const {
            return Engine::FileSystem::getPath(path);
        }
        std::vector<std::string> getAbsPaths() const {
            std::vector<std::string> tmp;
            for (auto path : paths) {
                tmp.push_back(Engine::FileSystem::getPath(path));
            }
            return tmp;
        }
    };
}

#endif //ASSETOBJECT_H
