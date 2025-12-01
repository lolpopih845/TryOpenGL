//
// Created by User1 on 1/12/2025.
//

#include "Rigidbody.h"

#include "transform.h"

namespace Components {
    const char * Rigidbody::getComponentName() const {
        return Component::getComponentName();
    }

    void Rigidbody::addForce(const glm::vec3 &f)  {
        forces += f;
    }

    void Rigidbody::update(float dt) {
        if (isKinematic) return;

        auto* transform = gameObject->getComponent<Transform>();
        if (!transform) return;

        glm::vec3 acceleration = forces / mass;

        if (useGravity) acceleration += glm::vec3(0,-9.81f,0);

        velocity += acceleration * dt;

        velocity *= (1.0f - friction * dt);

        transform->getGlobalTransform().translation += velocity * dt;

        forces = glm::vec3(0);
    }
} // Components