#ifndef SCENE_H
#define SCENE_H
#include "ObjectManager.h"
#include "CameraMan.h"
#include "StageDirector.h"

namespace Engine {

class Scene {
    public:
    ObjectManager objects;
    CameraMan cameras;
    StageDirector lighting;

    explicit Scene(SceneData data): lighting(StageDirector(data)) {
        objects = ObjectManager();
        cameras = CameraMan();
        objects.scene = this;
        cameras.scene = this;
        lighting.scene = this;
        lighting.Init();
        cameras.CreateCamera("MainCamera");
    }

    void Activate() {
        currentScene = this;
    }
    static Scene* GetCurrentScene() { return currentScene; }
private:
    static inline Scene* currentScene = nullptr;
};

} // Engine

#endif //SCENE_H
