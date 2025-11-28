//
// Created by User1 on 15/11/2025.
//

#include "camera.h"
#include <glm/gtc/type_ptr.inl>
#include <glm/gtc/matrix_transform.hpp>

#include "../Engine/EngineUtils.h"
#include "../Engine/GameObject.h"

namespace Components {

    Camera::Camera(glm::vec2 viewPortOffset, const glm::vec2 viewPortSize, glm::vec3 up, float yaw, float pitch) : Position(),
        Front(
            glm::vec3(0.0f, 0.0f, -1.0f)), Up(up), Right(glm::vec3(0)), transform(nullptr) {
        this->viewPortOffset = viewPortOffset;
        this->viewPortSize = viewPortSize;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        projection = glm::mat4(1.f);
        //updateCameraVectors();
    }


    void Camera::init() {
        SetUpCameraPerspective(glm::radians(45.0f),800 / 600,0.1f,10.f);
        setupShaderCameraBuffer();
        transform = gameObject->getComponent<Transform>();
    }

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::SetUpCameraPerspective(float fovY, const float aspect,const float nearr, const float farr) {
    projection = glm::perspective(fovY, aspect, nearr, farr);
}

void Camera::setupShaderCameraBuffer() {
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4) + sizeof(glm::vec4));
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);
}

    void Camera::cameraRender(float dtime) {
        //Create New ViewPort
        glViewport(cameraViewPort[0], cameraViewPort[1], cameraViewPort[2], cameraViewPort[3]);

        //Clear GPU
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //SetupMatrix
        Position = transform->getGlobalTransform().translation;
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up    = glm::normalize(glm::cross(Right, Front));
        if (UBO == 0) return;
        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(GetViewMatrix()));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
        glm::vec4 camPos(Position,1.0f);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*2, sizeof(glm::vec4), glm::value_ptr(camPos));

        //Set Listener
        const irrklang::vec3df s_pos = {Position.x,Position.y,Position.z};
        const irrklang::vec3df s_look = {Front.x,Front.y,Front.z};
        Game::GetSoundEngine()->setListenerPosition(s_pos, s_look,{0,0,0},{0,1,0});

        //Render Skybox
        Game::CAM.GetSkybox()->render(this);

        //Render Object --> ObjectManager
    }


void Camera::update(float dtime) {
    //Nothing


}
    const char *Camera::getComponentName() const {
        return "Camera";
    }

    std::ostream& operator<<(std::ostream& os, const Camera& c) {
        os << "Camera (" << c.gameObject->name << "): \n"
        << "Position: [" << c.Position.x << ", " << c.Position.y << ", " << c.Position.z << "]" << "\n"
        << "Front: [" << c.Front.x << ", " << c.Front.y << ", " << c.Front.z << "]" << "\n"
        << "Up: [" << c.Up.x << ", " << c.Up.y << ", " << c.Up.z << "]" << "\n"
        << "Right: [" << c.Right.x << ", " << c.Right.y << ", " << c.Right.z << "]" << "\n"
        << "WorldUp: [" << c.WorldUp.x << ", " << c.WorldUp.y << ", " << c.WorldUp.z << "]" << "\n"
        << "Yaw: " << c.Yaw << "\n"
        << "Pitch: " << c.Pitch << "\n";
        return os;
    }
} // Components