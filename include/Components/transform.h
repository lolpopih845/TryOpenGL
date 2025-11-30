#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "component.h"


#include "../Engine/GameObject.h"
namespace Components{
    class Transform : public Component {
    public:
        bool updated;

        explicit Transform(Engine::Transform local = Engine::DEFAULT_TRANSFORM);
        Engine::Transform getTransform() const;
        Engine::Transform getGlobalTransform() const;
        void setTransform(const Engine::Transform &local);
        void setPosition(glm::vec3 t);
        void setRotation(glm::vec3 t);
        void setScale(glm::vec3 t);
        void update(float dTime) override;
        const char *getComponentName() const override;
        friend std::ostream& operator<<(std::ostream& os, const Transform& transform);
    private:
        Engine::Transform local;
        Engine::Transform global;
    };
}




#endif //TRANSFORM_H
