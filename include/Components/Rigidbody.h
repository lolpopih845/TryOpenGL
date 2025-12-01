#ifndef RIGIBODY_H
#define RIGIBODY_H

#include "component.h"
#include "../Engine/glLib.h"
namespace Components {

class Rigidbody:public Component{
public:
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 forces   = glm::vec3(0.0f);

    float mass = 1.0f;
    float bounce = 0.1f;
    float friction = 0.2f;

    bool useGravity = true;
    bool isKinematic = false;

    const char* getComponentName() const override;

    void addForce(const glm::vec3& f);

    void update(float dt) override;
};

} // Components

#endif //RIGIBODY_H
