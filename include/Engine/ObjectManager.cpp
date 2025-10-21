#include "ObjectManager.h"

namespace Engine {
    void ObjectManager::DestroyObject(GameObject *obj) {
        auto removeObj = [obj](auto& list) {
            for (auto it = list.begin(); it != list.end(); ++it)
                if (it->get() == obj) {
                    list.erase(it);
                    return true;
                }
            return false;
        };

        if (removeObj(active_objects)) return;
        removeObj(pending_objects);
    }

    void ObjectManager::UpdateAll(const float dTime) {
        for (size_t i = 0; i < pending_objects.size(); ++i) {
            pending_objects[i]->init();
            active_objects.push_back(std::move(pending_objects[i]));
        }
        pending_objects.clear();
        for (const auto& obj : active_objects)
            if (obj->isActive())
                obj->update(dTime);
    }
}

