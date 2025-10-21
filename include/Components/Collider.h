#ifndef COLLIDER_H
#define COLLIDER_H
#include <unordered_set>

#include "component.h"
#include "transform.h"

namespace Components {
    class Collider : public Component {
    public:
        glm::vec3 center{0.0f};
        glm::vec3 size{1.0f};
        bool isTrigger = false;
        Engine::Transform transform;
        std::unordered_set<Collider*> currentCollisions;

        void init() override;

        ~Collider() override;

        bool intersects(const Collider &other) const;

        void handleCollision(Collider *other);

        void checkEndedCollisions();

        virtual void onCollisionEnter(Collider* other) {};
        virtual void onCollisionStay(Collider* other) {};
        virtual void onCollisionExit(Collider* other) {};

        void onEnable() override;
        void onDisable() override;
    };
}

#endif //COLLIDER_H
