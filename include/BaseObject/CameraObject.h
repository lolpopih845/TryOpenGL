#ifndef CAMERAOBJECT_H
#define CAMERAOBJECT_H
#include "../Components/camera.h"
#include "../Engine/GameObject.h"

// Do not regist to ObjectManager -> Regist to CameraMan

namespace Prefab {

class CameraObject :public Engine::GameObject{
public:
    explicit CameraObject(const Engine::GameObjectID id,std::string& name, Engine::Transform transform = Engine::DEFAULT_TRANSFORM, Engine::GameObjectID parent = Engine::INVALID_ID,
                            glm::vec2 viewPortOffset  = glm::vec2(0.0f),glm::vec2 viewPortSize = glm::vec2(1.0f),
                            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                          float yaw = -90,
                          float pitch = 0) : GameObject(id, name,transform,parent){
        addComponent<Components::Camera>(viewPortOffset,viewPortSize, up, yaw, pitch);
    }
};

} // Prefab

#endif //CAMERAOBJECT_H
