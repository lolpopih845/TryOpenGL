#ifndef CAMERAOBJECT_H
#define CAMERAOBJECT_H
#include "../Components/camera.h"
#include "../Engine/GameObject.h"

namespace Prefab {

class CameraObject :public Engine::GameObject{
public:
    explicit CameraObject(GameObject* parent = nullptr,
                          glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                          glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                          float yaw = -90,
                          float pitch = 0) : GameObject(*parent){
        addComponent<Components::Camera>(position, up, yaw, pitch);
    }
};

} // Prefab

#endif //CAMERAOBJECT_H
