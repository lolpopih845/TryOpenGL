#include "CameraMan.h"

#include "ObjectManager.h"

namespace Engine {
    void CameraMan::AddCamera(Prefab::CameraObject* camera) {
        if (camera->name=="") {
            camera->name = "Camera"+std::to_string(cameras.size());
        }
        cameras.push_back(std::move(camera));
    }

    void CameraMan::RemoveCamera(const int cam) {
        cameras.erase(cameras.begin() + cam);
    }

    Prefab::CameraObject* CameraMan::GetCamera(const int cam) {
        return cameras[cam];
    }

    Prefab::CameraObject* CameraMan::GetCamera(const std::string &name) {
        for (const auto& cam : cameras) {
            if (cam->name == name) {
                return cam;
            }
        }
        return nullptr;
    }

    void CameraMan::AdjustCameraViewPort(GLFWwindow *window, int width, int height) {
        mainViewport.width = width;
        mainViewport.height = height;
        for(auto& camObj : cameras) {
            const auto cam = camObj->getComponent<Components::Camera>();
            const auto camViewSize = glm::vec2(mainViewport.width * cam->viewPortSize.x, mainViewport.height*cam->viewPortSize.y);
            const auto camViewOffset = glm::vec2(camViewSize.x * cam->viewPortOffset.x, camViewSize.y * cam->viewPortOffset.y);
            const auto camViewPosition = glm::vec2(mainViewport.width/2 - camViewSize.x/2 + camViewOffset.x, mainViewport.height/2 - camViewSize.y/2 + camViewOffset.y);
            cam->cameraViewPort[0] = camViewPosition.x;
            cam->cameraViewPort[1] =camViewPosition.y;
            cam->cameraViewPort[2] = camViewSize.x;
            cam->cameraViewPort[3] = camViewSize.y;
            cam->SetUpCameraPerspective(glm::radians(45.0f),camViewSize.x / camViewSize.y,0.1f,10.f);
        }
    }
}


