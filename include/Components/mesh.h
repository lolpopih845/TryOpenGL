#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include "../Asset/shader.h"
#include "../Asset/texture.h"
#include <vector>
#include "component.h"
#include "transform.h"

#define MAX_BONE_INFLUENCE 4
namespace Engine {
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;

        int m_BoneIDs[MAX_BONE_INFLUENCE];
        float m_Weights[MAX_BONE_INFLUENCE];
    };
}
namespace Components {
    class Mesh : public Renderer {
    public:
        std::vector<Engine::Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Asset::Texture> textures;
        unsigned int VAO;

        explicit Mesh(const std::vector<Engine::Vertex> &vertices = {},
                      const std::vector<unsigned int> &indices = {},
                      const std::vector<Asset::Texture> &textures = {},
                      bool use_tangent = false,
                      bool use_skinning = false);

        void setupMesh(const std::vector<Engine::Vertex> &vertices,
                       const std::vector<unsigned int> &indices,
                       const std::vector<Asset::Texture> &textures,
                      bool use_tangent = false,
                      bool use_skinning = false);
        void updateMeshes(bool use_tangent = false, bool use_skinning=false);
        void render() override;


    private:
        unsigned int VBO, EBO;
        bool use_tangent;
        bool use_skinning;

        void init() override;
    };
}


#endif