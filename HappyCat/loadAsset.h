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

    Engine::Transform transform{
        glm::vec3(0, 2, -5), // position
        glm::vec3(0, 0, 0),  // rotation
        glm::vec3(1, 1, 1)   // scale
    };

    Game::CreateObject<Prefab::Player>("CatBoi", transform);

    // Asset::Animation walkAnimation("Walking", "resources/models/happyCat/Catboi_Walk.fbx");
    // Asset::AssetStorage<Asset::Animation>::Load(walkAnimation);
}

#endif //LOADASSET_H
