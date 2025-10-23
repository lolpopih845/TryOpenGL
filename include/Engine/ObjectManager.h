#pragma once
#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <vector>
#include <memory>
#include "GameObject.h"

namespace Engine {
    class ObjectManager {
    public:
        template <typename T, typename ... Args>
        static T* CreateObject(Args&& ...args) {
            auto obj = std::make_unique<T>(std::forward<Args>(args)...);
            T* ptr = obj.get();
            pending_objects.push_back(std::move(obj));
            return ptr;
        }
        static void DestroyObject(GameObject* obj);
        static void UpdateAll(float dTime);
    private:
        static inline std::vector<std::unique_ptr<GameObject>> active_objects;
        static inline std::vector<std::unique_ptr<GameObject>> pending_objects;
        static inline std::vector<GameObject*> pending_destroy;

        static void CleanupDestroyedObjects();
    };

}


#endif //OBJECTMANAGER_H
