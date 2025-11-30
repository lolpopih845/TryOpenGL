#ifndef SPRITE_H
#define SPRITE_H

#include "../../include/BaseObject/Mesh.h"

namespace Prefab {

    class Sprite:public MeshObject{
    public:
        explicit Sprite(
            const Engine::GameObjectID id, const std::string &name, const Engine::Transform &transform = Engine::DEFAULT_TRANSFORM, const Engine::GameObjectID parent = Engine::INVALID_ID,
            Asset::Texture* texture): MeshObject(id,name,transform,parent) {
            Components::Mesh* mesh = getComponent<Components::Mesh>();
            std::vector<Engine::Vertex>().swap(mesh->vertices);
            std::vector<unsigned int>().swap(mesh->indices);
            std::vector<Asset::Texture*>().swap(mesh->textures);
            const float vert[] = {
                -1, -1, 0,   0,  0, -1,   0, 0,
                 1, -1, 0,   0,  0, -1,   1, 0,
                 -1, 1, 0,   0,  0, -1,   0, 1,
                 1,  1, 0,   0,  0, -1,   1, 1,
            };

            for (int i = 0;i < std::size(vert); i+=8) {
                mesh->vertices.push_back({
                    glm::vec3(vert[i], vert[i + 1], vert[i + 2]), glm::vec3(vert[i + 3], vert[i + 4], vert[i + 5]),
                    glm::vec2(vert[i + 6], vert[i + 7])
                });
            }

            mesh->indices = {
                0, 1, 2,
                1, 2, 3,
            };
            mesh->textures.push_back(texture);
        }
    };

} // Prefab

#endif //SPRITE_H
