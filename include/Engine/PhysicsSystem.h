#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <unordered_map>
#include <vector>

#include "GameObjDef.h"
#include "glm/vec3.hpp"

namespace Components {
    class Collider;
    class Transform;
    class Rigibody;
}

namespace Engine {
    struct PhysicsData {
        GameObjectID id;
        Components::Collider* collider;
        Components::Transform* transform;
        Components::Rigibody* rigid;
    };
    class PhysicsSystem {
    public:
        static void RegisterPhysics(GameObjectID id);

        static void PhysicsUpdate(float dTime);
    private:
        static std::vector<PhysicsData> physics_object;
        static std::unordered_map<GameObjectID, PhysicsData> physicsMap;
    };

}

#endif //PHYSICSSYSTEM_H

/*
 class PhysicsSystem {
    public:
        static std::vector<Components::Collider*> colliders;
        static std::vector<Components::Collider*> pending_destroy;

        static void RegisterCollider(Components::Collider* c);

        static void UnregisterCollider(Components::Collider* c);

        static void Update(float dTime) ;
    };
 */