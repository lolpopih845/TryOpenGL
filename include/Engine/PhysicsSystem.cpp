//
// Created by User1 on 21/10/2025.
//

#include "PhysicsSystem.h"
#include <algorithm>

#include "EngineUtils.h"
#include "../Components/Collider.h"

namespace Engine {
    void PhysicsSystem::RegisterPhysics(const GameObjectID id) {
        // Check Dup
        if (const auto it = physicsMap.find(id); it != physicsMap.end()) {
            return;
        }

        const auto go = Game::Get(id); // your engine’s lookup

        PhysicsData data;
        data.id = id;
        data.collider = go->getComponent<Components::Collider>();
        data.rigid = go->getComponent<Components::Rigibody>();
        data.transform = go->getComponent<Components::Transform>();
        physics_object.push_back(data);
        physicsMap[id] = data;
    }

    void PhysicsSystem::PhysicsUpdate(float dTime) {
        //Get Object or Destroy
        for (int i = 0; i < physics_object.size(); ) {
            const auto obj = Game::Get(physics_object[i].id);

            //Check Destroyed
            if (!obj) {
                physics_object[i] = physics_object.back();
                physics_object.pop_back();
                physicsMap.erase(physics_object[i].id);
                continue;
            }

            //Check Active
            if (!obj->active) { continue; }

            //Collider
            if (physics_object[i].collider && physics_object[i].collider->isEnabled()) {
                for (size_t j = i + 1; j < colliders.size(); ++j) {
                    auto* A = colliders[i];
                    if (auto* B = colliders[j]; A->intersects(*B)) {
                        A->handleCollision(B);
                        B->handleCollision(A);
                    }
                }
            }

            i++;
        }


        for (size_t i = 0; i < colliders.size(); ++i) {

        }
        for (auto* c : colliders) {
            c->checkEndedCollisions();
        }
    }
} // Engine