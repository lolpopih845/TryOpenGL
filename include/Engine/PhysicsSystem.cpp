//
// Created by User1 on 21/10/2025.
//

#include "PhysicsSystem.h"
#include <algorithm>
#include <iostream>

#include "../Components/Collider.h"

namespace Engine {
    std::vector<Components::Collider*> PhysicsSystem::colliders;
    std::vector<Components::Collider*> PhysicsSystem::pending_destroy;
    void PhysicsSystem::RegisterCollider(Components::Collider *c) {
        colliders.push_back(c);
    }

    void PhysicsSystem::UnregisterCollider(Components::Collider *c){
        colliders.erase(std::remove(colliders.begin(), colliders.end(), c), colliders.end());
    }

    void PhysicsSystem::Update(float dTime) {
        for (size_t i = 0; i < colliders.size(); ++i) {
            for (size_t j = i + 1; j < colliders.size(); ++j) {
                auto* A = colliders[i];
                if (auto* B = colliders[j]; A->intersects(*B)) {
                    A->handleCollision(B);
                    B->handleCollision(A);
                }
            }
        }
        for (auto* c : colliders) {
            c->checkEndedCollisions();
        }
    }
} // Engine