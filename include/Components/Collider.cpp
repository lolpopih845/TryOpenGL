//
// Created by User1 on 21/10/2025.
//

#include "Collider.h"

#include <iostream>
#include <ostream>

#include "../Engine/PhysicsSystem.h"

namespace Components {
    void Collider::init() {
        transform = gameObject->getComponent<Transform>();
        Engine::PhysicsSystem::RegisterCollider(this);
    }

    Collider::Collider(glm::vec3 center, glm::vec3 size) {
        this->center = center;
        this->size = size;
    }

    Collider::~Collider() {
        for (auto* other : currentCollisions)
            other->currentCollisions.erase(this);
        Engine::PhysicsSystem::UnregisterCollider(this);
    }

    bool Collider::intersects(const Collider &other) const {
        glm::vec3 aMin = transform->getGlobalTransform().translation + center - size * 0.5f;
        glm::vec3 aMax = transform->getGlobalTransform().translation + center + size * 0.5f;
        glm::vec3 bMin = other.transform->getGlobalTransform().translation + other.center - other.size * 0.5f;
        glm::vec3 bMax = other.transform->getGlobalTransform().translation + other.center + other.size * 0.5f;
        return (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
               (aMin.y <= bMax.y && aMax.y >= bMin.y) &&
               (aMin.z <= bMax.z && aMax.z >= bMin.z);
    }
    void Collider::handleCollision(Collider* other) {
        if (currentCollisions.find(other) == currentCollisions.end()) {
            for (const auto &comp : gameObject->components) {
                comp->onCollisionEnter(other);
            }
            currentCollisions.insert(other);
        } else {
            for (const auto &comp : gameObject->components) {
                comp->onCollisionStay(other);
            }
        }
    }
    void Collider::checkEndedCollisions() {
        std::vector<Collider*> ended;
        for (Collider* c : currentCollisions) {
            if (!intersects(*c)) {
                ended.push_back(c);
                for (const auto &comp : gameObject->components) {
                    comp->onCollisionExit(c);
                }
            }
        }
        for (Collider* c : ended)
            currentCollisions.erase(c);
    }

    void Collider::onEnable() {
        Engine::PhysicsSystem::RegisterCollider(this);
    }

    void Collider::onDisable() {
        Engine::PhysicsSystem::UnregisterCollider(this);
        currentCollisions.clear();
    }

} // namespace Components