#ifndef ASSETSTORAGE_H
#define ASSETSTORAGE_H
#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
namespace Asset {
    template<typename T, typename... Args>
class AssetStorage {
public:
    static std::shared_ptr<T> Load(Args&&... args) {
        static_assert(std::is_base_of<AssetObject, T>::value,
                 "T must derive from AssetObject");
        try {
            auto asset = std::make_shared<T>(std::forward<Args>(args)...);
            if (assets().count(asset->name)) {
                std::cerr << "Asset already exists: " << asset->name << std::endl;
                return assets()[asset->name];
            }
            assets()[asset->name] = asset;
            return asset;
        }
        catch (const std::exception& e) {
            std::cerr << "Wrong parameter or Asset Error" << std::endl;
            return nullptr;
        }


    }

    static std::shared_ptr<T> Get(const std::string& id) {
        auto& map = assets();
        auto it = map.find(id);
        if (it == map.end()) {
            std::cerr << "No such asset" << std::endl;
            return nullptr;
        }
        return std::dynamic_pointer_cast<T>(it->second);
    }

private:
    static std::unordered_map<std::string, std::shared_ptr<AssetObject>>& assets() {
        static std::unordered_map<std::string, std::shared_ptr<AssetObject>> instance;
        return instance;
    }
};

} // Asset

#endif //ASSETSTORAGE_H
