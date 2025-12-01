//
// Created by User1 on 21/10/2025.
//

#include "PhysicsSystem.h"

#include "EngineUtils.h"
#include "../Components/Collider.h"
#include "../Components/Rigidbody.h"

namespace Engine {
    std::vector<PhysicsData> PhysicsSystem::physics_object;
    std::unordered_map<GameObjectID, PhysicsData> PhysicsSystem::physicsMap;

    void PhysicsSystem::RegisterPhysics(const GameObjectID id) {
        // Check Dup
        if (const auto it = physicsMap.find(id); it != physicsMap.end()) {
            return;
        }

        const auto go = Game::Get(id); // your engine’s lookup

        PhysicsData data;
        data.id = id;
        data.collider = go->getComponent<Components::Collider>();
        data.rigid = go->getComponent<Components::Rigidbody>();
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

    bool ComputeAABBDistance(const Components::Collider* A, const Components::Collider* B,
                         glm::vec3& outPenetration) {

        const Transform tA = A->gameObject->getComponent<Components::Transform>()->getGlobalTransform();
        const Transform tB = B->gameObject->getComponent<Components::Transform>()->getGlobalTransform();

        glm::vec3 aMin = tA.translation + A->center - A->size * 0.5f;
        glm::vec3 aMax = tA.translation + A->center + A->size * 0.5f;

        glm::vec3 bMin = tB.translation + B->center - B->size * 0.5f;
        glm::vec3 bMax = tB.translation + B->center + B->size * 0.5f;

        // Compute overlap on each axis
        float dx = std::min(aMax.x, bMax.x) - std::max(aMin.x, bMin.x);
        float dy = std::min(aMax.y, bMax.y) - std::max(aMin.y, bMin.y);
        float dz = std::min(aMax.z, bMax.z) - std::max(aMin.z, bMin.z);

        if (dx <= 0 || dy <= 0 || dz <= 0)
            return false; // no penetration

        // Choose the smallest axis to resolve first
        if (dx < dy && dx < dz)
            outPenetration = { dx, 0, 0 };
        else if (dy < dz)
            outPenetration = { 0, dy, 0 };
        else
            outPenetration = { 0, 0, dz };

        // Direction: push A away from B
        glm::vec3 centerA = (aMin + aMax) * 0.5f;
        glm::vec3 centerB = (bMin + bMax) * 0.5f;

        outPenetration *= glm::sign(centerA - centerB);

        return true;
    }

    void RigidCheck(const Components::Collider* A, const Components::Collider* B) {
        auto* rMoving = A->gameObject->getComponent<Components::Rigidbody>();
        if (!rMoving || rMoving->isKinematic) return; // only move if it has a rigidbody

        glm::vec3 penetration;
        if (!ComputeAABBDistance(A, B, penetration))
            return;

        glm::vec3 normal = glm::normalize(penetration);

        // Compute velocity along normal
        float vn = glm::dot(rMoving->velocity, normal);

        // Only respond if moving into the other object
        if (vn < 0) {
            // Reflect velocity based on bounce
            rMoving->velocity -= (1.0f + rMoving->bounce) * vn * normal;

            // Apply friction along other axes
            rMoving->velocity -= rMoving->velocity * rMoving->friction;
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

        for (auto& p : physics_object) {
            if (!p.rigid) continue;
            if (!p.transform) continue;

            p.rigid->update(dTime);
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

                        RigidCheck(A,B);

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