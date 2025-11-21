#ifndef ASSETSTORAGE_H
#define ASSETSTORAGE_H
#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
namespace Asset {
    template<typename T>
class AssetStorage {
public:
        template<typename... Args>
    static T* Load(Args&&... args) {
        static_assert(std::is_base_of_v<AssetObject, T>,
                 "T must derive from AssetObject");
        try {
            auto asset = std::make_unique<T>(std::forward<Args>(args)...);
            auto& map = assets();
            if (map.count(asset->name)) {
                std::cerr << "Asset already exists: " << asset->name << std::endl;
                return static_cast<T*>(map[asset->name].get());
            }
            T* ptr = asset.get();
            map[asset->name] = std::move(asset);
            return ptr;
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return nullptr;
        }


    }

    static T* Get(const std::string& id) {
        auto& map = assets();
        const auto it = map.find(id);
        if (it == map.end()) {
            std::cerr << "No such asset" << std::endl;
            return nullptr;
        }
        return static_cast<T*>(it->second.get());
    }

private:
    static std::unordered_map<std::string, std::unique_ptr<AssetObject>>& assets() {
        static std::unordered_map<std::string, std::unique_ptr<AssetObject>> instance;
        return instance;
    }
};

} // Asset

#endif //ASSETSTORAGE_H
