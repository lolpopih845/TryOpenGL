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

struct Object3D {
    glm::vec3 position;
    glm::vec3 rotation; // in degrees
    glm::vec3 scale;
};

inline Object3D cube;

inline unsigned int shaderProgram = 0;

inline unsigned int CompileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION\n" << infoLog << std::endl;
    }

    return shader;
}

inline unsigned int CreateShaderProgram() {
    const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

    const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(0.8, 0.2, 0.2, 1.0); // red
}
)";
    unsigned int vertex = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}

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

    float vertices[] = {
        // positions
        -0.5f, -0.5f, -0.5f, // 0
         0.5f, -0.5f, -0.5f, // 1
         0.5f,  0.5f, -0.5f, // 2
        -0.5f,  0.5f, -0.5f, // 3
        -0.5f, -0.5f,  0.5f, // 4
         0.5f, -0.5f,  0.5f, // 5
         0.5f,  0.5f,  0.5f, // 6
        -0.5f,  0.5f,  0.5f  // 7
    };

    unsigned int indices[] = {
        // back face
        0, 1, 2,
        2, 3, 0,
        // front face
        4, 5, 6,
        6, 7, 4,
        // left face
        4, 0, 3,
        3, 7, 4,
        // right face
        1, 5, 6,
        6, 2, 1,
        // bottom face
        4, 5, 1,
        1, 0, 4,
        // top face
        3, 2, 6,
        6, 7, 3
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // vertex attrib
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    cube.position = glm::vec3(0.0f, 0.1f, -0.25f); // ตัวอย่างตำแหน่งในโลก
    cube.rotation = glm::vec3(0.0f); // หมุน 30° X, 45° Y
    cube.scale    = glm::vec3(0.1f); // ขนาดเดิม

    shaderProgram = CreateShaderProgram();

    while (!glfwWindowShouldClose(window)) {
        GameUpdate(window);

        // ใช้ shader program
        glUseProgram(shaderProgram);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cube.position);
        model = glm::rotate(model, glm::radians(cube.rotation.x), glm::vec3(1,0,0));
        model = glm::rotate(model, glm::radians(cube.rotation.y), glm::vec3(0,1,0));
        model = glm::rotate(model, glm::radians(cube.rotation.z), glm::vec3(0,0,1));
        model = glm::scale(model, cube.scale);

        // world space
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);

        // view + projection camera (เหมือนเดิม)
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0));
        glm::mat4 projection = glm::perspective(glm::radians(-45.0f), 800.0f/600.0f, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
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

    auto camID = Engine::Scene::GetCurrentScene()->cameras.GetCurrentCamera();
    auto camObj = Engine::Scene::GetCurrentScene()->objects.Get(camID);
    auto cam = camObj->getComponent<Components::Camera>();
    cam->ProcessMouseMovement(Engine::InputSystem::getMouseOffset().x, Engine::InputSystem::getMouseOffset().y);

    if(Engine::InputSystem::isKeyHold(Engine::KeyCode::W)) cam->ProcessKeyboard(0, deltaTime);
    if(Engine::InputSystem::isKeyHold(Engine::KeyCode::S)) cam->ProcessKeyboard(1, deltaTime);
    if(Engine::InputSystem::isKeyHold(Engine::KeyCode::A)) cam->ProcessKeyboard(2, deltaTime);
    if(Engine::InputSystem::isKeyHold(Engine::KeyCode::D)) cam->ProcessKeyboard(3, deltaTime);

    // std::cout << "Camera Position: "
    //       << "X: " << cam->Position.x << ", "
    //       << "Y: " << cam->Position.y << ", "
    //       << "Z: " << cam->Position.z << std::endl;

    // glfwSwapBuffers(window);
    // glfwPollEvents();
}

inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //All camera update width, height
    glViewport(0, 0, width, height);
    AdjustCameraViewPort(window, width, height);

}