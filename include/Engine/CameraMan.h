#ifndef CAMERAMAN_H
#define CAMERAMAN_H
#include <vector>
#include "glLib.h"
#include "../BaseObject/CameraObject.h"
#include "../Asset/texture.h"

namespace Engine{
    class Scene;

    struct ViewPort {
        static inline int width = 800;
        static inline int height = 600;
    };
    class SkyBox {
    public:
        Asset::Texture* texture;
        Asset::Shader* shader;
        unsigned int VAO;
        explicit SkyBox();
        void render(const Components::Camera* camera) const;
    private:
        std::vector<float> skyboxVertices;
        unsigned int VBO;
    };
class CameraMan {
public:
    Scene* scene;
    void CreateCamera(const std::string& name, Transform transform = DEFAULT_TRANSFORM, GameObjectID parent = INVALID_ID,
                            glm::vec2 viewPortOffset  = glm::vec2(0.0f),glm::vec2 viewPortSize = glm::vec2(1.0f),
                            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                          float yaw = -90,
                          float pitch = 0);
    void RemoveCamera(int cam);
    GameObjectID GetCamera(int cam = 0) const;

    GameObjectID GetCamera(const std::string &name) const;
    SkyBox* GetSkybox(){ return &skybox; }
    void AdjustCameraViewPort(GLFWwindow* window, int width, int height);
    ViewPort mainViewport;
private:
    std::vector<GameObjectID> cameras;
    SkyBox skybox;
};

} // Engine

#endif //CAMERAMAN_H
