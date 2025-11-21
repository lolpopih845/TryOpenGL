#ifndef ENGINEUTILS_H
#define ENGINEUTILS_H
#include "Scene.h"

namespace Game {
    inline auto& OBJ = Engine::Scene::GetCurrentScene()->objects;

    inline auto& Get = OBJ.Get;

    template <typename T, typename... Args>
    Engine::GameObjectID CreateObject(Args&&... args) {
        return OBJ.CreateObject<T>(std::forward<Args>(args)...);
    }

    inline auto DestroyObject = [](const Engine::GameObjectID id) {
        OBJ.DestroyObject(id);
    };
    inline auto SetParentObj = [](const Engine::GameObjectID child, const Engine::GameObjectID parent) {
        OBJ.SetParent(child, parent);
    };
    inline auto AddChild = [](const Engine::GameObjectID parent, const Engine::GameObjectID child) {
        OBJ.AddChild(parent, child);
    };
    inline auto RemoveChild = [](const Engine::GameObjectID parent, const Engine::GameObjectID child) {
        OBJ.RemoveChild(parent, child);
    };

    inline auto& CAM = Engine::Scene::GetCurrentScene()->cameras;

    inline auto CreateCamera = [](const std::string& name, const Engine::Transform &transform = Engine::DEFAULT_TRANSFORM, const Engine::GameObjectID parent = Engine::INVALID_ID,
                                glm::vec2 viewPortOffset  = glm::vec2(0.0f),glm::vec2 viewPortSize = glm::vec2(1.0f),
                                glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                              float yaw = -90,
                              float pitch = 0) {
        CAM.CreateCamera(name, transform, parent, viewPortOffset, viewPortSize, up, yaw, pitch);
    };

    inline auto RemoveCamera = [](const int cam) {
        CAM.RemoveCamera(cam);
    };

    inline auto GetCamera = [](const int cam = 0) {
        return CAM.GetCamera(cam);
    };

    inline auto GetCameraByName = [](const std::string &name) {
        return CAM.GetCamera(name);
    };

    inline auto AdjustCameraViewPort = [](GLFWwindow* window, int width, int height) {
        CAM.AdjustCameraViewPort(window, width, height);
    };

    inline auto& SCENE_DIRECTOR = Engine::Scene::GetCurrentScene()->lighting;
}



#endif //ENGINEUTILS_H
