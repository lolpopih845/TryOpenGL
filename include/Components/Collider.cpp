//
// Created by User1 on 21/10/2025.
//

#include "Collider.h"

#include <ostream>

#include "../Engine/PhysicsSystem.h"

namespace Components {

    void Collider::init() {
        transform = gameObject->getComponent<Transform>();
        Engine::PhysicsSystem::RegisterPhysics(gameObject->id);
    }

    Collider::Collider(glm::vec3 center, glm::vec3 size) {
        this->center = center;
        this->size = size;
    }

    Collider::~Collider() {
        for (auto* other : currentCollisions)
            other->currentCollisions.erase(this);
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

    void Collider::onDisable() {
        currentCollisions.clear();
    }

    const char * Collider::getComponentName() const {
        return "Collider";
    }

    std::ostream& operator<<(std::ostream& os, const Collider& collider) {
        os << "Collider (" << collider.gameObject->name << "): \n"
        << "center: [" << collider.center[0] << ", " << collider.center[1] << ", " << collider.center[2] << "\n"
        << "size: [" << collider.size[0] << ", " << collider.size[1] << ", " << collider.size[2] << "]\n"
        << "isTrigger: " << collider.isTrigger << "\n"
        << "currentCollisions: ";
        for (const auto &c : collider.currentCollisions) os<< c->gameObject->name << ", ";os << std::endl;
        return os;
    }
} // namespace Components


