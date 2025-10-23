#include "transform.h"

namespace Components {
    Transform::Transform(Engine::Transform local) : updated(false), local(local), global(Engine::DEFAULT_TRANSFORM) {}

    Engine::Transform Transform::getTransform() const {
        return local;
    }

    Engine::Transform Transform::getGlobalTransform() const {
        return global;
    }

    void Transform::setTransform(Engine::Transform local) {
        this->local = local;
        updated = false;
    }

    void Transform::setPosition(const glm::vec3 t) {
        local.translation = t;
        updated = false;
    }

    void Transform::setRotation(const glm::vec3 t) {
        local.rotation = t;
        updated = false;
    }

    void Transform::setScale(const glm::vec3 t) {
        local.scale = t;
        updated = false;
    }

    void Transform::update(float dTime){
        if (updated) return;
        const Transform* parentTransform = nullptr;

        if (gameObject->parent)
            parentTransform = gameObject->parent->getComponent<Transform>();

        if (parentTransform) {
            global.scale = parentTransform->global.scale * local.scale;
            global.rotation = parentTransform->global.rotation + local.rotation;
            global.translation = glm::quat(glm::radians(parentTransform->global.rotation)) * (parentTransform->global.scale * local.translation) + parentTransform->global.translation;
        }
        else
            global = local;

        updated = true;

        for (const auto& child : gameObject->children)
            child->getComponent<Transform>()->updated = false;
    }
}
