//
// Created by User1 on 21/10/2025.
//

#include "PhysicsSystem.h"

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

    void InsertToGrid(Components::Collider* c) {
        const Transform t = c->gameObject->getComponent<Components::Transform>()->getGlobalTransform();
        glm::vec3 aMin = t.translation + c->center - c->size * 0.5f;
        glm::vec3 aMax = t.translation + c->center + c->size * 0.5f;
        int minX = floor(aMin.x / CELL_SIZE);
        int minY = floor(aMin.y / CELL_SIZE);
        int minZ = floor(aMin.z / CELL_SIZE);

        int maxX = floor(aMax.x / CELL_SIZE);
        int maxY = floor(aMax.y / CELL_SIZE);
        int maxZ = floor(aMax.z / CELL_SIZE);

        for (int x = minX; x <= maxX; x++)
            for (int y = minY; y <= maxY; y++)
                for (int z = minZ; z <= maxZ; z++) {
                    grid[{x,y,z}].push_back(c);
                }
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
            i++;
        }

        //Collider
        BuildGrid();

        std::unordered_set<uint64_t> testedPairs;
        for (auto& [cell, list] : grid) {
            const size_t n = list.size();
            for (size_t j = 0; j < n; j++) {
                for (size_t k = j + 1; k < n; k++) {

                    Components::Collider* A = list[j];
                    Components::Collider* B = list[k];

                    uint64_t key = ((uint64_t)std::min(A,B) << 32) | ((uint64_t)std::max(A,B) & 0xffffffff);
                    if (testedPairs.count(key)) continue;
                    testedPairs.insert(key);

                    if (A->intersects(*B)) {
                        A->handleCollision(B);
                        B->handleCollision(A);
                    }
                }
            }
        }

        for (const auto c : physics_object)
            c.collider->checkEndedCollisions();
    }

    void PhysicsSystem::BuildGrid() {
        grid.clear();
        for (const auto& p : physics_object) {
            InsertToGrid(p.collider);
        }
    }
} // Engine