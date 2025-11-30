#ifndef MESHOBJECT_H
#define MESHOBJECT_H
#include "../Components/mesh.h"
#include "../Engine/GameObject.h"

namespace Prefab {

class MeshObject:public Engine::GameObject{
public:
    explicit MeshObject(
               const Engine::GameObjectID id, const std::string &name, const Engine::Transform &transform = Engine::DEFAULT_TRANSFORM, const Engine::GameObjectID parent = Engine::INVALID_ID,
               const std::vector<Engine::Vertex>& vertices = {},
               const std::vector<unsigned int>& indices = {},
               const std::vector<Asset::Texture*>& textures = {},
               bool use_tangent = false,
               bool use_skinning = false): GameObject(id, name,transform, parent)
    {
        addComponent<Components::Mesh>(vertices, indices, textures, use_tangent, use_skinning);
    }
};

} // Prefab

#endif //MESHOBJECT_H
