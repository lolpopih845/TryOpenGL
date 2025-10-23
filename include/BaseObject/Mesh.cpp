//
// Created by User1 on 20/10/2025.
//

#include "Mesh.h"

namespace Prefab {
    MeshObject::MeshObject(GameObject* parent,
                       const std::vector<Engine::Vertex>& vertices,
                       const std::vector<unsigned int>& indices,
                       const std::vector<Asset::Texture>& textures,
                       bool use_tangent,
                       bool use_skinning)
    : GameObject(parent)
    {
        addComponent<Components::Mesh>(vertices, indices, textures, use_tangent, use_skinning);
    }
} // Prefab