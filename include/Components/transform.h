#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../Engine/GameObject.h"
#include "iostream"
namespace Engine {
    struct Transform {
        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::mat4 getMatrix() const {
            auto m = glm::mat4(1.0f);
            m = glm::translate(m, translation);
            m = glm::rotate(m, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            m = glm::rotate(m, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            m = glm::rotate(m, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            m = glm::scale(m, scale);
            return m;
        }
    };

    const Transform DEFAULT_TRANSFORM({glm::vec3(0.0f, 0.0f, 0.f), glm::vec3(.0f, 0.0f, 0.0f),glm::vec3(1.0f,1.0f,1.0f)});
    inline std::ostream& operator<<(std::ostream& os, const Transform& t) {
        os << "Translation: [" << t.translation[0] << ", " << t.translation[1] << ", " << t.translation[2] << "] "
        << "Rotation: [" << t.rotation[0] << ", " << t.rotation[1] << ", " << t.rotation[2] << "] "
        << "Scale: [" << t.scale[0] << ", " << t.scale[1] << ", " << t.scale[2] << "] ";
        return os;
    }
}


namespace Components{
    class Transform : public Component {
    public:
        bool updated;

        explicit Transform(Engine::Transform local = Engine::DEFAULT_TRANSFORM);
        Engine::Transform getTransform() const;
        Engine::Transform getGlobalTransform() const;
        void setTransform(Engine::Transform local);
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
