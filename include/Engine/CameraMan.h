#ifndef CAMERAMAN_H
#define CAMERAMAN_H
#include <memory>
#include <vector>
#include "glLib.h"
#include "../BaseObject/CameraObject.h"

namespace Engine{
    struct ViewPort {
        static inline int width = 800;
        static inline int height = 600;
    };
class CameraMan {
public:
    static void AddCamera(Prefab::CameraObject* camera);
    static void RemoveCamera(int cam);
    static Prefab::CameraObject* GetCamera(int cam = 0);

    static Prefab::CameraObject *GetCamera(const std::string &name);

    static void AdjustCameraViewPort(GLFWwindow* window, int width, int height);
    static ViewPort mainViewport;
private:
    static inline std::vector<Prefab::CameraObject*> cameras;
};

} // Engine

#endif //CAMERAMAN_H
