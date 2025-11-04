#ifndef ASSET_H
#define ASSET_H
#pragma once
#include <string>
#include <unordered_map>
#include <memory>
namespace Asset {
template<typename T>
class Asset {
public:
    static std::shared_ptr<T> Load(const std::string& id, std::shared_ptr<T> asset) {
        assets()[id] = asset;
        return asset;
    }

    static std::shared_ptr<T> Get(const std::string& id) {
        auto& map = assets();
        auto it = map.find(id);
        if (it != map.end()) return it->second;
        return nullptr;
    }

private:
    static std::unordered_map<std::string, std::shared_ptr<T>>& assets() {
        static std::unordered_map<std::string, std::shared_ptr<T>> instance;
        return instance;
    }
};

} // Asset

#endif //ASSET_H
