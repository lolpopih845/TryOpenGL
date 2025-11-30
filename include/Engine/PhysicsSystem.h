#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <unordered_map>
#include <vector>

#include "GameObjDef.h"

namespace Components {
    class Collider;
    class Transform;
    // class Rigibody;
}

namespace Engine {

    //Spatial Cell
    struct CellCoord {
        int x, y, z;
        bool operator==(const CellCoord& o) const {
            return x == o.x && y == o.y && z == o.z;
        }
    };

    struct CellHash {
        size_t operator()(const CellCoord& c) const {
            // Good enough simple hash
            return (c.x * 73856093) ^ (c.y * 19349663) ^ (c.z * 83492791);
        }
    };

    static std::unordered_map<CellCoord, std::vector<Components::Collider*>, CellHash> grid;
    inline static float CELL_SIZE = 2.0f;

    struct PhysicsData {
        GameObjectID id;
        Components::Collider* collider;
        Components::Transform* transform;
        // Components::Rigibody* rigid;
    };

    class PhysicsSystem {
    public:
        static void RegisterPhysics(GameObjectID id);

        static void PhysicsUpdate(float dTime);



    private:
        static std::vector<PhysicsData> physics_object;
        static std::unordered_map<GameObjectID, PhysicsData> physicsMap;

        static void BuildGrid();

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