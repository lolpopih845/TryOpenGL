#include "GameObject.h"

#include <iostream>
namespace Engine {
    GameObject::GameObject(const std::string& name, const Transform &transform, const GameObjectID parent):name(name),parent(parent) {
        addComponent<Components::Transform>()->setTransform(transform);

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

    std::ostream& operator<<(std::ostream& os, const GameObject& go) {
        os << go.name <<": parent: " << go.parent.idx << " tag: " << go.tag << std::endl;
        os << "Children: [";
        for (auto& c : go.children) { os << c.idx << ", "; }
        os << "]" << std::endl;
        os << "Components: [";
        for (auto& c : go.components) { os << c->getComponentName() << ", ";}
        os << "]" << std::endl;
        return os;
    }
}
