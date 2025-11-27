#include "ObjectManager.h"
#include <memory>
#include "CameraMan.h"

namespace Engine {
    template<typename T, typename ... Args>
    GameObjectID ObjectManager::CreateObject(Args &&...args) {
        {
            uint32_t idx;
            if (!freeSpaceList.empty()) {
                idx = freeSpaceList.back();
                freeSpaceList.pop_back();
            }
            else {
                idx = active_objects.size();
                active_objects.push_back({});
            }
            const GameObjectID id{ idx, active_objects[idx].gen };
            active_objects[idx].obj = std::make_unique<T>(id,std::forward<Args>(args)...);
            pending_objects.push_back(id);
            return id;
        }
    }

    void ObjectManager::DestroyObject(const GameObjectID id) {
        if (const auto obj = Get(id)) {
            obj->destroyed = true;
        }
        pending_destroy.push_back(id);
    }

    void ObjectManager::UpdateAll(const float dTime) {
        //Init
        for (size_t i = 0; i < pending_objects.size(); ++i) {
            if (const auto obj = Get(pending_objects[i])) {
                obj->init();
            }
        }
        pending_objects.clear();
        //Update
        for (const auto& [obj,gen] : active_objects)
            if (obj->isActive()&&!obj->destroyed)
                obj->update(dTime);
        //Destroy
        CleanupDestroyedObjects();
    }

    GameObject * ObjectManager::Get(const GameObjectID id) const {
        if (id.idx >= active_objects.size()) return nullptr;
        auto &slot = active_objects[id.idx];

        if (slot.gen != id.gen) return nullptr;

        return slot.obj.get();
    }

    void ObjectManager::SetParent(const GameObjectID child, const GameObjectID parent) const {
        GameObject* c = Get(child);
        GameObject* p = Get(parent);
        if (!c || !p) return;

        if (IsIDValid(c->parent)) {
            if (Get(c->parent))
                RemoveChild(c->parent, child);
        }
        c->parent = parent;
        p->children.push_back(child);
    }

    void ObjectManager::AddChild(const GameObjectID parent, const GameObjectID child) const {
        GameObject* p = Get(parent);
        GameObject* c = Get(child);
        if (!p || !c) return;

        p->children.push_back(child);
        c->parent = parent;
    }

    void ObjectManager::RemoveChild(const GameObjectID parent, const GameObjectID child) const {
        GameObject* p = Get(parent);
        GameObject* c  = Get(child);

        if (!p || !c) return;

        p->children.erase(std::remove(p->children.begin(), p->children.end(), c), p->children.end());

        if (c->parent.idx == parent.idx &&
            c->parent.gen == parent.gen)
        {
            c->parent = INVALID_ID;
        }
    }

    void ObjectManager::CleanupDestroyedObjects() {
        size_t i = 0;
        while (i < pending_destroy.size()) {
            GameObjectID id = pending_destroy[i];

            if (id.idx < active_objects.size() &&
                active_objects[id.idx].gen == id.gen &&
                active_objects[id.idx].obj) {
                GameObject* obj = active_objects[id.idx].obj.get();

                for (auto childID : obj->children)
                {
                    if (childID.idx < active_objects.size())
                        pending_destroy.push_back(childID);
                }
                active_objects[id.idx].obj.reset();
                active_objects[id.idx].gen++;
                freeSpaceList.push_back(id.idx);
            }
            ++i;
        }
        pending_destroy.clear();

    }


}

