#ifndef LOADASSET_H
#define LOADASSET_H
#include "../include/Asset/animation.h"
#include "../include/Asset/AssetStorage.h"
#include "../include/Asset/ModelLoader.h"
#include "../include/BaseObject/ModelObject.h"
#include "../include/Engine/EngineUtils.h"
#include "Prefab/Player.h"

inline void LoadAsset() {
    // Asset::Shader _3DShader("3Ds","shaders/3Dv.hlsl", "shaders/3Df.hlsl");
    // Asset::Shader skyboxShader("SkyBoxs","shaders/SkyBoxV.glsl", "shaders/SkyBoxF.glsl");
    // Asset::Shader boneShader("BoneS","shaders/BoneV.hlsl", "shaders/BoneF.hlsl");
    //
    // Asset::Texture so_true("sotrue","resources/sotrue.png");
    //
    // monster = Asset::ModelLoader::LoadModelGameObject("MixamoBoy","resources/objects/mixamoBoy/Neutral.fbx",&boneShader,true);
    // Asset::AssetStorage<Asset::Animation>::Load(Asset::LoadAnimation("Neutral","resources/objects/mixamoBoy/Neutral.fbx"));
    // Asset::AssetStorage<Asset::Animation>::Load(Asset::LoadAnimation("Walking","resources/objects/mixamoBoy/Walking.fbx"));
    // monster->addComponent<Components::MixamoBoy>();
    // monster->getComponent<Components::MixamoBoy>()->camera = &camera;
    // monster->name = "monster";

    Engine::Transform transform1{
        glm::vec3(0.0f, 0.0f, 2.0f), // position
        glm::vec3(0.0f),  // rotation
        glm::vec3(10.0f)   // scale
    };
    Engine::Transform transform2{
        glm::vec3(2.0f, 0.0f, 0.0f), // position
        glm::vec3(0.0f),  // rotation
        glm::vec3(10.0f)   // scale
    };
    Engine::Transform transform3{
        glm::vec3(-2.0f, 0.0f, 0.0f), // position
        glm::vec3(0.0f),  // rotation
        glm::vec3(10.0f)   // scale
    };
    Engine::Transform transform4{
        glm::vec3(0.0f, 0.0f, -2.0f), // position
        glm::vec3(0.0f),  // rotation
        glm::vec3(10.0f)   // scale
    };

    Game::CreateObject<Prefab::Player>("CatBoi1", transform1);
    Game::CreateObject<Prefab::Player>("CatBoi2", transform2);
    Game::CreateObject<Prefab::Player>("CatBoi3", transform3);
    Game::CreateObject<Prefab::Player>("CatBoi4", transform4);

    // Asset::Animation walkAnimation("Walking", "resources/models/happyCat/Catboi_Walk.fbx");
    // Asset::AssetStorage<Asset::Animation>::Load(walkAnimation);
}

#endif //LOADASSET_H
