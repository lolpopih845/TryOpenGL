#include "InputSystem.h"

#include "ObjectManager.h"


namespace Engine {
    void InputSystem::updateTrackKey(KeyCode key) {
        if (keyStateCurr.find(key)==keyStateCurr.end()) {
            keyStateCurr[key] = false;
            keyStatePrev[key] = false;
        }
    }
    bool InputSystem::isKeyPressed(KeyCode key) {
        updateTrackKey(key);
        return keyStateCurr[key]&&!keyStatePrev[key];
    }

    bool InputSystem::isKeyHold(KeyCode key) {
        updateTrackKey(key);
        return keyStateCurr[key]&&keyStatePrev[key];
    }

    bool InputSystem::isKeyReleased(KeyCode key) {
        updateTrackKey(key);
        return !keyStateCurr[key]&&keyStatePrev[key];
    }

    void InputSystem::updateTrackMouse(MouseCode m) {
        if (mouseStateCurr.find(m)==mouseStateCurr.end()) {
            mouseStateCurr[m] = false;
            mouseStatePrev[m] = false;
        }
    }

    bool InputSystem::isMousePressed(MouseCode button) {
        return mouseStateCurr[button] && !mouseStatePrev[button];
    }

    bool InputSystem::isMouseHold(MouseCode button) {
        return mouseStateCurr[button] && mouseStatePrev[button];
    }

    bool InputSystem::isMouseReleased(MouseCode button) {
        return !mouseStateCurr[button] && mouseStatePrev[button];
    }

    void InputSystem::ProcessInputFromGLFW(GLFWwindow *window) {
        InputSystem::window = window;
        for (auto& [key,value]: keyStateCurr) {
            keyStatePrev[key] = keyStateCurr[key];
            keyStateCurr[key] = (glfwGetKey(window, static_cast<int>(key)) == GLFW_PRESS);
        }

        for (auto& [key,value]: mouseStateCurr) {
            mouseStatePrev[key] = mouseStateCurr[key];
            mouseStateCurr[key] = (glfwGetMouseButton(window, static_cast<int>(key)) == GLFW_PRESS);
        }


        double x, y;
        glfwGetCursorPos(window, &x, &y);
        if (mousePosition == glm::vec2{-9999,-99999} ) {
            mouseLastPosition = {x, y};
        }
        mouseLastPosition = mousePosition;
        mousePosition = glm::vec2(x, y);
    }

    glm::vec2 InputSystem::getMousePosition() {
        return mousePosition;
    }

    glm::vec2 InputSystem::getMouseOffset() {
        float xoffset = mousePosition.x - mouseLastPosition.x;
        float yoffset = mouseLastPosition.y - mousePosition.y; // reversed since y-coordinates go from bottom to top
        return {xoffset,yoffset};
    }

    void InputSystem::setCursorMode(CursorMode cursor) {
        glfwSetInputMode(window, GLFW_CURSOR, static_cast<int>(cursor));
    }

} // Engine