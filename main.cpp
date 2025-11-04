#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/Asset/shader.h"
#include "include/Asset/texture.h"
#include "include/camera.h"
#include "HappyForm.h"
#include "include/Engine/PhysicsSystem.h"

#include <iostream>

#include "include/Asset/AnimLoader.h"
#include "include/Asset/asset.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

//GameObject Keep
Engine::GameObject* trumpet = nullptr;
Engine::GameObject* monster = nullptr;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    camera.setupShaderCameraBuffer();

    // build and compile our shader program
    // ------------------------------------
    Asset::Shader _3DShader("shaders/3Dv.hlsl", "shaders/3Df.hlsl");
    Asset::Shader skyboxShader("shaders/SkyBoxV.hlsl", "shaders/SkyBoxF.hlsl");
    Asset::Shader boneShader("shaders/BoneV.hlsl", "shaders/BoneF.hlsl");

    Asset::Texture so_true("resources/sotrue.png");

    float merlinSpawnTime = 0.0f;
    std::vector<Asset::Texture> textures;
    textures.push_back(so_true); //Will be later save in asset.h
    FrickingSkyBox fsb = CreateSkyBox(skyboxShader);
    //trumpet = CreateTrumpet(&camera);

    monster = Asset::ModelLoader::LoadModelGameObject("resources/objects/mixamoBoy/Neutral.fbx",&boneShader,"mixamoBoy");
    Asset::Asset<Asset::Animation>::Load("Neutral",Asset::LoadAnimation("resources/objects/mixamoBoy/Neutral.fbx"));
    Asset::Asset<Asset::Animation>::Load("Walking",Asset::LoadAnimation("resources/objects/mixamoBoy/Walking.fbx"));
    monster->addComponent<Components::MixamoBoy>();
    monster->getComponent<Components::MixamoBoy>()->camera = &camera;
    monster->name = "monster";

    camera.SetUpCameraPerspective(glm::radians(45.0f),(float)SCR_WIDTH / (float)SCR_HEIGHT,0.1f,10.f);
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        const float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);
        //Light
        _3DShader.use();
        _3DShader.setVec3("viewPos", camera.Position);
        _3DShader.setFloat("material.shininess", 100.f);
        //Dir light
        _3DShader.setVec3("dirLight.direction", -20.f, -10.0f, -20.f);
        _3DShader.setVec3("dirLight.ambient", 1.f, 1.f, 1.f);
        _3DShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        _3DShader.setVec3("dirLight.specular", 0.f, 0.f, 0.f);
        //Spotlight
        _3DShader.setVec3("spotLight.position", camera.Position);
        _3DShader.setVec3("spotLight.direction", camera.Front);
        _3DShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        _3DShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        _3DShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        _3DShader.setFloat("spotLight.constant", 1.0f);
        _3DShader.setFloat("spotLight.linear", 0.09f);
        _3DShader.setFloat("spotLight.quadratic", 0.032f);
        _3DShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        _3DShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        skyboxShader.setFloat("time",glfwGetTime());

        //render
        glClearColor(1.f-0.1*glm::min(5.f,currentFrame), 0.9f-0.09*glm::min(5.f,currentFrame), 1.f-0.1*glm::min(5.f,currentFrame), 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        so_true.bind();
        Engine::PhysicsSystem::Update(deltaTime);
        Engine::ObjectManager::UpdateAll(deltaTime);

        UpdateSkyBox(fsb,camera);

        if (merlinSpawnTime<0.0f) {
            merlinSpawnTime = 5.f-currentFrame/100.0f;
            CreateMerlin(&_3DShader,monster,currentFrame);
        }
        merlinSpawnTime -= deltaTime;

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        monster->getComponent<Components::MixamoBoy>()->accel[2] = 5;
        //camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        monster->getComponent<Components::MixamoBoy>()->accel[2] = -5;
        //camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        monster->getComponent<Components::MixamoBoy>()->accel[0] = -5;
        //camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        monster->getComponent<Components::MixamoBoy>()->accel[0] = 5;
        //camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    // if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    //     trumpet->getComponent<Components::Tp>()->Fire();
    // }
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
