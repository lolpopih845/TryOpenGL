//
// Created by User1 on 4/11/2025.
//

#include "../Engine/EngineUtils.h"
#include "Model.h"

namespace Components {
    void Model::UpdateMeshChildren() {
        const std::vector<Engine::GameObjectID> children = gameObject->getChildren();
        meshes.clear();
        for (const Engine::GameObjectID id: children) {
            if (Engine::GameObject* child = Game::Get(id); child->getComponent<Mesh>()) {
                meshes.push_back(child);
            }
        }
    }

    Engine::GameObject* Model::GetMesh(unsigned int index) const {
        if (index >= meshes.size())
            return nullptr;
        return meshes[index];
    }

    // Move out all meshes
    std::vector<Engine::GameObject *> Model::GetMeshes()
    {
        return meshes;
    }

    // Apply shader to each mesh
    void Model::SetShader(Asset::Shader* shader) {
        for (auto& m : meshes)
        {
            if (m)
                m->getComponent<Mesh>()->SetShader(shader); // assumes Mesh has SetShader
        }
        this->shader = shader;
    }

    Asset::Shader* Model::GetShader() const {
        return shader;
    }

    void Model::AddMesh(Engine::GameObject *mesh) {
        meshes.push_back(mesh);
    }

    const char * Model::getComponentName() const {
        return "Model";
    }
} // Components