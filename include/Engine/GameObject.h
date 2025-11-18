#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <memory>
#include <vector>
#include "../Components/component.h"
#include "../Components/transform.h"
//namespace Components { class Transform; }

namespace Engine {
    class GameObject {
    public:
        bool active = true;
        GameObject* parent = nullptr;
        std::string name;
        // Tag tag;
        std::vector<GameObject*> children;
        std::vector<std::unique_ptr<Components::Component>> components;

        explicit GameObject(const std::string& name,GameObject* parent = nullptr);

        void addChild(GameObject* child);

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

        std::vector<Components::Component*> getAllComponents() {
            std::vector<Components::Component*> result;
            for (auto& c : components)
                result.push_back(c.get());
            return result;
        }

        std::vector<GameObject*> getChildren() {
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
