#pragma once
#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <vector>
#include "GameObjDef.h"


namespace Engine {
    class Scene;

    class ObjectManager {
    public:
        Scene* scene;
        template <typename T, typename ... Args>
        GameObjectID CreateObject(Args&& ...args);
        void DestroyObject(GameObjectID id);
        void UpdateAll(float dTime);
        GameObject* Get(GameObjectID id) const;
        void SetParent(GameObjectID child, GameObjectID parent) const;
        void AddChild(GameObjectID parent, GameObjectID child) const;
        void RemoveChild(GameObjectID parent, GameObjectID child) const;
    private:
        std::vector<GameObjectSlot> active_objects;
        std::vector<uint32_t> freeSpaceList;
        std::vector<GameObjectID> pending_objects;
        std::vector<GameObjectID> pending_destroy;

        void CleanupDestroyedObjects();
    };


}


#endif //OBJECTMANAGER_H
