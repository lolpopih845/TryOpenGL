#ifndef COMPONENT_H
#define COMPONENT_H
#include "../Asset/shader.h"

namespace Engine {class GameObject;}
namespace Components {
    class Collider;
    class Component {
    public:
        Engine::GameObject* gameObject = nullptr;
        virtual ~Component() = default;
        virtual void update(float dTime) {}
        virtual void init() {}
        void setEnabled(const bool e) {
            if (enabled == e) return;
            enabled = e;
            if (enabled) onEnable();
            else onDisable();
        }
        virtual void onEnable() {}
        virtual void onDisable() {}

        //Collider
        virtual void onCollisionEnter(Collider* other) {}
        virtual void onCollisionStay(Collider* other) {}
        virtual void onCollisionExit(Collider* other) {}

        //Trigger
        virtual void onTriggerEnter(Collider* other) {}
        virtual void onTriggerStay(Collider* other) {}
        virtual void onTriggerExit(Collider* other) {}

        bool isEnabled() const { return enabled; }
        virtual const char* getComponentName() const { return "Unnamed_Component"; }
    private:
        bool enabled = true;
    };

    class Renderer : public Component {
    public:
        Asset::Shader* shader;
        explicit Renderer() : shader(&Asset::DefaultShader()) {}
        virtual void SetShader(Asset::Shader* shader) {
            this->shader = shader;
        }
        virtual void render() {}
    private:

    };
}


#endif //COMPONENT_H
