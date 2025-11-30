#ifndef CAMERA_H
#define CAMERA_H
#include "component.h"
#include "transform.h"

namespace Components {

class Camera : public Component{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Yaw;
    float Pitch;
    glm::vec2 viewPortOffset = glm::vec2(0.0f);
    glm::vec2 viewPortSize = glm::vec2(1.0f);
    float cameraViewPort[4] = { 0, 0, 800, 600 };
    Transform* transform;

    explicit Camera(glm::vec2 viewPortOffset  = glm::vec2(0.0f),glm::vec2 viewPortSize = glm::vec2(1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90, float pitch = 0);

    glm::mat4 GetViewMatrix() const;
    void SetUpCameraPerspective(float fovY, float aspect, float near, float far);
    void setupShaderCameraBuffer();
    void init() override;
    void cameraRender();
    void update(float dtime) override;
    const char *getComponentName() const override;
    friend std::ostream& operator<<(std::ostream& os, const Camera& camera);
private:
    unsigned int UBO = 0;
    glm::mat4 projection;
};

} // Components

#endif //CAMERA_H
