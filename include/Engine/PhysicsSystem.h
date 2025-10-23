#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <vector>

namespace Components {
    class Collider;
}

namespace Engine {
    class PhysicsSystem {
    public:
        static std::vector<Components::Collider*> colliders;
        static std::vector<Components::Collider*> pending_destroy;

        static void RegisterCollider(Components::Collider* c);

        static void UnregisterCollider(Components::Collider* c);

        static void Update(float dTime) ;
    };

}

#endif //PHYSICSSYSTEM_H
