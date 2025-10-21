#ifndef MESHOBJECT_H
#define MESHOBJECT_H
#include "../Components/mesh.h"
#include "../Engine/GameObject.h"

namespace Prefab {

class MeshObject:public Engine::GameObject{
public: // <- make constructor public
    MeshObject(GameObject* parent = nullptr,
               const std::vector<Engine::Vertex>& vertices = {},
               const std::vector<unsigned int>& indices = {},
               const std::vector<Asset::Texture>& textures = {},
               bool use_tangent = false,
               bool use_skinning = false);
};

} // Prefab

#endif //MESHOBJECT_H
