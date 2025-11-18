#ifndef COLLIDER_H
#define COLLIDER_H
#include <unordered_set>

#include "component.h"
#include "transform.h"

namespace Components {
    class Collider : public Renderer {
    public:
        glm::vec3 center{0.0f};
        glm::vec3 size{1.0f};
        bool isTrigger = false;
        Transform* transform = nullptr;
        std::unordered_set<Collider*> currentCollisions;

        void init() override;

        explicit Collider(glm::vec3 center = glm::vec3(0), glm::vec3 size= glm::vec3(1));

        ~Collider() override;

        bool intersects(const Collider &other) const;

        void handleCollision(Collider *other);

        void checkEndedCollisions();

        void onEnable() override;
        void onDisable() override;
        const char *getComponentName() const override;
        friend std::ostream& operator<<(std::ostream& os, const Collider& collider);
    };
}

#endif //COLLIDER_H
