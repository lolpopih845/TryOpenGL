#pragma once
#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <vector>

#include "GameObject.h"
#include "renameLater.h"

namespace Engine {


    class ObjectManager {
    public:
        template <typename T, typename ... Args>
        static GameObjectID CreateObject(Args&& ...args);
        static void DestroyObject(GameObjectID id);
        static void UpdateAll(float dTime);
        static GameObject* Get(GameObjectID id);
        static void SetParent(GameObjectID child, GameObjectID parent);
        static void AddChild(GameObjectID parent, GameObjectID child);
        static void RemoveChild(GameObjectID parent, GameObjectID child);
    private:
        static inline std::vector<GameObjectSlot> active_objects;
        static inline std::vector<GameObjectID> pending_objects;
        static inline std::vector<GameObjectID> pending_destroy;

        static void CleanupDestroyedObjects();
    };


}


#endif //OBJECTMANAGER_H
