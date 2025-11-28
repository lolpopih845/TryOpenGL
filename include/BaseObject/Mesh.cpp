//
// Created by User1 on 20/10/2025.
//

#include "Mesh.h"

namespace Prefab {
    MeshObject::MeshObject(
        Engine::GameObjectID id, const std::string& name, Engine::Transform transform, const Engine::GameObjectID parent,
                       const std::vector<Engine::Vertex>& vertices,
                       const std::vector<unsigned int>& indices,
                       const std::vector<Asset::Texture>& textures,
                       bool use_tangent,
                       bool use_skinning)
    : GameObject(id, name,transform, parent)
    {
        addComponent<Components::Mesh>(vertices, indices, textures, use_tangent, use_skinning);
    }
} // Prefab