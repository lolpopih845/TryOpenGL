#ifndef LOADASSET_H
#define LOADASSET_H
#include "../include/Asset/AssetStorage.h"
#include "../include/Asset/ModelLoader.h"
#include "../include/Engine/EngineUtils.h"

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

    // TODO() Add folder name happyCat at /resources/model, and then add Arm.fbx and CatBoi.fbx inside it.
    Engine::GameObject* catBoiGO = new Engine::GameObject(catBoiID, "catBoi", defaultTransform);
    Game::Scene::GetCurrentScene()->objects.Add(catBoiGO);
    catBoi = Asset::ModelLoader::LoadModelGameObject("catBoi", "resources/models/happyCat/Catboi.fbx", "", true);
}

#endif //LOADASSET_H
