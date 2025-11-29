#ifndef SPHERE_H
#define SPHERE_H
#include "../../include/BaseObject/Mesh.h"

namespace Prefab {

    class Sphere:public MeshObject{
    public:
        explicit Sphere(
            const Engine::GameObjectID id, const std::string &name, const Engine::Transform &transform = Engine::DEFAULT_TRANSFORM, const Engine::GameObjectID parent = Engine::INVALID_ID,
            const float radius = 1, const unsigned int sectorCount = 50, const unsigned int stackCount = 25
            ): MeshObject(id,name,transform,parent) {
            Components::Mesh* mesh = getComponent<Components::Mesh>();
            //----------------------- Vertice -----------------------------------------
            std::vector<Engine::Vertex>().swap(mesh->vertices);
            float x, y, z, xy; // vertex position
            float nx, ny, nz, lengthInv = 1.0f / radius; // vertex normal
            float s, t; // vertex texCoord

            //PosStepCal
            float sectorStep = 2 * glm::pi<float>() / sectorCount;
            float stackStep = glm::pi<float>() / stackCount;
            float sectorAngle, stackAngle;

            for (int i = 0; i <= stackCount; ++i) {
                stackAngle = glm::pi<float>() / 2 - i * stackStep; // starting from pi/2 to -pi/2
                xy = radius * cosf(stackAngle); // r * cos(u)
                z = radius * sinf(stackAngle); // r * sin(u)

                // add (sectorCount+1) vertices per stack
                // first and last vertices have same position and normal, but different tex coords
                for (int j = 0; j <= sectorCount; ++j) {
                    sectorAngle = j * sectorStep; // starting from 0 to 2pi

                    // vertex position (x, y, z)
                    x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
                    y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
                    // normalized vertex normal (nx, ny, nz)
                    nx = x * lengthInv;
                    ny = y * lengthInv;
                    nz = z * lengthInv;

                    // vertex tex coord (s, t) range between [0, 1]
                    s = -(float) j / (sectorCount / 2);
                    t = (float) i / stackCount;
                    mesh->vertices.push_back({glm::vec3(x, y, z), glm::vec3(nx, ny, nz), glm::vec2(s, t)});
                }
            }

            //------------------------------- Indices ---------------------------------------
            std::vector<unsigned int>().swap(mesh->indices);
            int k1, k2;
            for (int i = 0; i < stackCount; ++i) {
                k1 = i * (sectorCount + 1); // beginning of current stack
                k2 = k1 + sectorCount + 1; // beginning of next stack

                for (int j = 0; j < (int) ((float) sectorCount); ++j, ++k1, ++k2) {
                    // 2 triangles per sector excluding first and last stacks
                    // k1 => k2 => k1+1
                    if (i != 0) {
                        mesh->indices.push_back(k1);
                        mesh->indices.push_back(k2);
                        mesh->indices.push_back(k1 + 1);
                    }

                    // k1+1 => k2 => k2+1
                    if (i != (stackCount - 1)) {
                        mesh->indices.push_back(k1 + 1);
                        mesh->indices.push_back(k2);
                        mesh->indices.push_back(k2 + 1);
                    }
                }
            }
            addComponent<Components::Collider>(glm::vec3(0),glm::vec3(1));
        }
    };

} // Prefab

#endif //SPHERE_H
