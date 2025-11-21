
#include "./include/Engine/glLib.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/Asset/shader.h"
#include "include/Asset/texture.h"
#include "HappyForm.h"
#include "include/Engine/PhysicsSystem.h"
#include "include/Engine/InputSystem.h"

#include <iostream>

#include "include/garbage/AnimLoader.h"
#include "include/Asset/AssetStorage.h"
#include "include/Engine/CameraMan.h"
#include "HappyCat/happycat.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
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

    //Setting Up Cam
    Engine::CameraMan::mainViewport.width = 800;
    Engine::CameraMan::mainViewport.height = 600;

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(Engine::CameraMan::mainViewport.width, Engine::CameraMan::mainViewport.height, "HappyCat", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    Engine::CameraMan::AddCamera(Engine::ObjectManager::CreateObject<Prefab::CameraObject>());

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    // ------------------------------------
    Asset::Shader _3DShader("3Ds","shaders/3Dv.hlsl", "shaders/3Df.hlsl");
    Asset::Shader skyboxShader("SkyBoxs","shaders/SkyBoxV.glsl", "shaders/SkyBoxF.glsl");
    Asset::Shader boneShader("BoneS","shaders/BoneV.hlsl", "shaders/BoneF.hlsl");

    Asset::Texture so_true("sotrue","resources/sotrue.png");

    float merlinSpawnTime = 0.0f;
    std::vector<Asset::Texture> textures;
    textures.push_back(so_true); //Will be later save in asset.h
    FrickingSkyBox fsb = CreateSkyBox(skyboxShader);
    //trumpet = CreateTrumpet(&camera);

    monster = Asset::ModelLoader::LoadModelGameObject("MixamoBoy","resources/objects/mixamoBoy/Neutral.fbx",&boneShader,true);
    Asset::AssetStorage<Asset::Animation>::Load(Asset::LoadAnimation("Neutral","resources/objects/mixamoBoy/Neutral.fbx"));
    Asset::AssetStorage<Asset::Animation>::Load(Asset::LoadAnimation("Walking","resources/objects/mixamoBoy/Walking.fbx"));
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
        Engine::InputSystem::ProcessInputFromGLFW(window);
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //All camera update width, height
    glViewport(0, 0, width, height);
}