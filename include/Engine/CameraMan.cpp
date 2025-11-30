#include "CameraMan.h"
#include "Scene.h"
#include "ObjectManager.h"

namespace Engine {
    void CameraMan::CreateCamera(const std::string &name, Transform transform, GameObjectID parent,
            glm::vec2 viewPortOffset, glm::vec2 viewPortSize, glm::vec3 up, float yaw, float pitch) {
        const GameObjectID id = scene->objects.CreateObject<Prefab::CameraObject>(name, transform, parent, viewPortOffset, viewPortSize, up, yaw, pitch);
        cameras.push_back(id);
    }

    void CameraMan::RemoveCamera(const int cam) {
        cameras.erase(cameras.begin() + cam);
    }

    GameObjectID CameraMan::GetCurrentCamera() const {
        return cameras[curr_cam];
    }

    GameObjectID CameraMan::GetCamera(const int cam) const {
        return cameras[cam];
    }

    GameObjectID CameraMan::GetCamera(const std::string &name) const {
        for (const auto& cam : cameras) {
            if (scene->objects.Get(cam)->name == name) {
                return cam;
            }
        }
        return INVALID_ID;
    }

    void CameraMan::AdjustCameraViewPort(GLFWwindow *window, int width, int height) {

        mainViewport.width = width;
        mainViewport.height = height;
        for(auto& camObj : cameras) {
            const auto cam = scene->objects.Get(camObj)->getComponent<Components::Camera>();
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

    SkyBox::SkyBox(): texture(&Asset::DefaultSkyBoxTexture()),shader(&Asset::SkyBoxShader()) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        skyboxVertices = {
            // positions
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f
        };
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    }

    void SkyBox::render(const Components::Camera* camera) const {
        if (!shader || VAO == 0) return;
        shader->use();
        shader->setInt("skybox", 0);
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        shader->setMat4("view", glm::mat4(glm::mat3(camera->GetViewMatrix())));
        // skybox cube
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture->ID);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }
}


