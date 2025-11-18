#ifndef ASSETOBJECT_H
#define ASSETOBJECT_H
#include <string>

namespace Asset {
    enum AssetType {
        TEXTURE,
        SHADER,
        MATERIAL,
        SKELETON,
        SOUND,
        ANIMATION
    };
    class AssetObject {
    public:
        std::string name;
        AssetType type;
        AssetObject(const std::string &name, const AssetType type) : name(name), type(type) {}
        virtual ~AssetObject() = default;
    };
}

#endif //ASSETOBJECT_H
