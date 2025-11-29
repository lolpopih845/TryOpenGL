#ifndef BOX_H
#define BOX_H

#ifndef SPHERE_H
#define SPHERE_H
#include "../../include/BaseObject/Mesh.h"

namespace Prefab {

    class Box:public MeshObject{
    public:
        explicit Box(
            const Engine::GameObjectID id, const std::string &name, const Engine::Transform &transform = Engine::DEFAULT_TRANSFORM, const Engine::GameObjectID parent = Engine::INVALID_ID
            ): MeshObject(id,name,transform,parent) {
            Components::Mesh* mesh = getComponent<Components::Mesh>();
            std::vector<Engine::Vertex>().swap(mesh->vertices);
            std::vector<unsigned int>().swap(mesh->indices);
            const float vert[] = {
                -1,  1, -1,   0,  0, -1,   0, 1,
                -1, -1, -1,   0,  0, -1,   0, 0,
                 1, -1, -1,   0,  0, -1,   1, 0,
                 1, -1, -1,   0,  0, -1,   1, 0,
                 1,  1, -1,   0,  0, -1,   1, 1,
                -1,  1, -1,   0,  0, -1,   0, 1,

                -1, -1,  1,  -1,  0,  0,   1, 0,
                -1, -1, -1,  -1,  0,  0,   0, 0,
                -1,  1, -1,  -1,  0,  0,   0, 1,
                -1,  1, -1,  -1,  0,  0,   0, 1,
                -1,  1,  1,  -1,  0,  0,   1, 1,
                -1, -1,  1,  -1,  0,  0,   1, 0,

                 1, -1, -1,   1,  0,  0,   0, 0,
                 1, -1,  1,   1,  0,  0,   1, 0,
                 1,  1,  1,   1,  0,  0,   1, 1,
                 1,  1,  1,   1,  0,  0,   1, 1,
                 1,  1, -1,   1,  0,  0,   0, 1,
                 1, -1, -1,   1,  0,  0,   0, 0,

                -1, -1,  1,   0,  0,  1,   0, 0,
                -1,  1,  1,   0,  0,  1,   0, 1,
                 1,  1,  1,   0,  0,  1,   1, 1,
                 1,  1,  1,   0,  0,  1,   1, 1,
                 1, -1,  1,   0,  0,  1,   1, 0,
                -1, -1,  1,   0,  0,  1,   0, 0,

                -1,  1, -1,   0,  1,  0,   0, 1,
                 1,  1, -1,   0,  1,  0,   1, 1,
                 1,  1,  1,   0,  1,  0,   1, 0,
                 1,  1,  1,   0,  1,  0,   1, 0,
                -1,  1,  1,   0,  1,  0,   0, 0,
                -1,  1, -1,   0,  1,  0,   0, 1,

                -1, -1, -1,   0, -1,  0,   0, 0,
                -1, -1,  1,   0, -1,  0,   0, 1,
                 1, -1, -1,   0, -1,  0,   1, 0,
                 1, -1, -1,   0, -1,  0,   1, 0,
                -1, -1,  1,   0, -1,  0,   0, 1,
                 1, -1,  1,   0, -1,  0,   1, 1
            };

            for (int i = 0;i < std::size(vert); i+=8) {
                mesh->vertices.push_back({
                    glm::vec3(vert[i], vert[i + 1], vert[i + 2]), glm::vec3(vert[i + 3], vert[i + 4], vert[i + 5]),
                    glm::vec2(vert[i + 6], vert[i + 7])
                });
            }

            mesh->indices = {
                0, 1, 2,
                3, 4, 5,

                6, 7, 8,
                9,10,11,

               12,13,14,
               15,16,17,


               18,19,20,
               21,22,23,

               24,25,26,
               27,28,29,

               30,31,32,
               33,34,35
            };
            addComponent<Components::Collider>(glm::vec3(0),glm::vec3(1));
        }
    };

} // Prefab

#endif //SPHERE_H


#endif //BOX_H
