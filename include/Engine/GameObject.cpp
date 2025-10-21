#include "GameObject.h"

#include <iostream>
namespace Engine {
    GameObject::GameObject(GameObject *parent) : parent(parent) {
        addComponent<Components::Transform>();
    }

    void GameObject::addChild(GameObject* child) {
        if (!child) return;
        child->parent = this;
        children.push_back(std::move(child));
    }

    void GameObject::init() const {
        for (auto& c : components) {
            c->init();
        }
    }


    void GameObject::update(float dTime) const {
        for (auto& c : components) {
            if (c->isEnabled()) {
                c->update(dTime);
                if (const auto r = dynamic_cast<Components::Renderer*>(c.get())) {
                    r->render();
                }
            }
        }
    }

    void GameObject::setActive(const bool active) {
        this->active = active;
    }

    bool GameObject::isActive() const {
        return this->active;
    }
}
