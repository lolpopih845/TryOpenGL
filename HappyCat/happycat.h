#pragma once

#include "loadAsset.h"
#include "../include/Engine/glLib.h"
#include "../include/Engine/Scene.h"
#include "../include/Engine/EngineUtils.h"

#include "../include/Engine/PhysicsSystem.h"
#include "../include/Engine/InputSystem.h"

using namespace Game;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void GameUpdate(GLFWwindow* window);

inline void GameInit() {
    //Setting Up Cam
    int width = 800;
    int height = 600;

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(800, 600, "HappyCat", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return ;
    }
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return ;
    }
    glViewport(0, 0, width, height);

    Engine::Scene mainScene = Engine::Scene({glm::vec4(1), glm::vec4(1), glm::vec4(1), glm::vec4(1)});
    mainScene.Activate();

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    AdjustCameraViewPort(window, width, height);


    glEnable(GL_DEPTH_TEST);

    LoadAsset();

    while (!glfwWindowShouldClose(window)) {
        GameUpdate(window);
    }

    glfwTerminate();
    return ;
}

inline float deltaTime = 0.0f;
inline float lastFrame = 0.0f;

inline void GameUpdate(GLFWwindow* window) {
    const float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    Engine::InputSystem::ProcessInputFromGLFW(window);

    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Engine::PhysicsSystem::PhysicsUpdate(deltaTime);
    Engine::Scene::GetCurrentScene()->objects.UpdateAll(deltaTime);
    Engine::Scene::GetCurrentScene()->cameras.RenderAll();

    // auto camID = Engine::Scene::GetCurrentScene()->cameras.GetCurrentCamera();
    // auto camObj = Engine::Scene::GetCurrentScene()->objects.Get(camID);
    // auto cam = camObj->getComponent<Components::Camera>();
    // cam->ProcessMouseMovement(Engine::InputSystem::getMouseOffset().x, Engine::InputSystem::getMouseOffset().y);
    //
    // if(Engine::InputSystem::isKeyHold(Engine::KeyCode::W)) cam->ProcessKeyboard(0, deltaTime);
    // if(Engine::InputSystem::isKeyHold(Engine::KeyCode::S)) cam->ProcessKeyboard(1, deltaTime);
    // if(Engine::InputSystem::isKeyHold(Engine::KeyCode::A)) cam->ProcessKeyboard(2, deltaTime);
    // if(Engine::InputSystem::isKeyHold(Engine::KeyCode::D)) cam->ProcessKeyboard(3, deltaTime);
    //
    // std::cout << "Camera Position: "
    //       << "X: " << cam->Position.x << ", "
    //       << "Y: " << cam->Position.y << ", "
    //       << "Z: " << cam->Position.z << std::endl;

    glfwSwapBuffers(window);
    glfwPollEvents();
}

inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //All camera update width, height
    glViewport(0, 0, width, height);
    AdjustCameraViewPort(window, width, height);

}