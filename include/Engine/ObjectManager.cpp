#include "ObjectManager.h"

#include <iostream>
#include "CameraMan.h"

namespace Engine {
    void ObjectManager::DestroyObject(GameObject* obj) {
        pending_destroy.push_back(obj);
    }

    void ObjectManager::UpdateAll(const float dTime) {
        //Init

        for (size_t i = 0; i < pending_objects.size(); ++i) {
            pending_objects[i]->init();
            active_objects.push_back(std::move(pending_objects[i]));
        }
        pending_objects.clear();
        //Update
        for (const auto& obj : active_objects)
            if (obj->isActive())
                obj->update(dTime);
        //Destroy
        CleanupDestroyedObjects();
    }

    std::vector<std::unique_ptr<GameObject>> ObjectManager::GetAllObjects() {
        return active_objects;
    }

    void ObjectManager::CleanupDestroyedObjects() {
        size_t i = 0;
        while (i < pending_destroy.size()) {
            auto* obj = pending_destroy[i];
            // Add children to pending_destroy
            for (auto* child : obj->children) {
                if (child) pending_destroy.push_back(child);
            }

            auto removeObj = [obj](auto& list) {
                for (auto it = list.begin(); it != list.end(); ++it) {
                    if (it->get() == obj) {
                        list.erase(it); // deletes object + triggers destructor
                        return true;
                    }
                }
                return false;
            };

            removeObj(active_objects);
            removeObj(pending_objects);

            ++i;
        }
        pending_destroy.clear(); // clear after all deletions

    }
}

