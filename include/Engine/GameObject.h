#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <memory>
#include <vector>
#include "../Components/component.h"
#include "GameObjDef.h"
namespace Components { class Transform; }

namespace Engine {

    struct GameObjectID;
    class GameObject {
    public:
        bool active = true;
        bool destroyed = false;
        GameObjectID id;
        std::string name;
        Tag tag;
        GameObjectID parent = INVALID_ID;
        std::vector<GameObjectID> children;
        std::vector<std::unique_ptr<Components::Component>> components;

        explicit GameObject(GameObjectID id, const std::string& name, const Transform &transform = DEFAULT_TRANSFORM,GameObjectID parent = INVALID_ID);

        template<typename T, typename... Args>
        T* addComponent(Args&&... args) {
                auto comp = std::make_unique<T>(std::forward<Args>(args)...);
                T* ptr = comp.get();
                ptr->gameObject = this;
                components.push_back(std::move(comp));
                return ptr;
        }

        template<typename T>
        T* getComponent() {
            for (auto& c : components)
                if (auto casted = dynamic_cast<T*>(c.get()))
                    return casted;
            return nullptr;
        }

        template<typename T>
        std::vector<T*> getComponents() {
            std::vector<T*> result;
            for (auto& c : components)
                if (auto casted = dynamic_cast<T*>(c.get()))
                    result.push_back(casted);
            return result;
        }

        std::vector<Components::Component*> getAllComponents() const {
            std::vector<Components::Component*> result;
            for (auto& c : components)
                result.push_back(c.get());
            return result;
        }

        std::vector<GameObjectID> getChildren() {
            return children;
        }

        void init() const;

        void update(float dTime) const;

        void setActive(bool active);

        bool isActive() const;

        friend std::ostream& operator<<(std::ostream& os, const GameObject& go);
    };

}



#endif //GAMEOBJECT_H
