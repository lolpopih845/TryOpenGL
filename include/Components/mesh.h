#ifndef MESH_H
#define MESH_H

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
    inline std::ostream& operator<<(std::ostream& os, const Vertex& v) {
        os << "{ position : [" << v.Position[0] << ", " << v.Position[1] << ", " << v.Position[2] <<"], "
            << "normal : [" << v.Normal[0] << ", " << v.Normal[1] << ", " << v.Normal[2] <<"], "
            << "texCoord : [" << v.TexCoords[0] << ", " << v.TexCoords[1] <<"], "
            << "tan : [" << v.Tangent[0] << ", " << v.Tangent[1] << ", " << v.Tangent[2] <<"], "
            << "biTan : [" << v.Bitangent[0] << ", " << v.Bitangent[1] << ", " << v.Bitangent[2] <<"], ";
        os << "boneID : [";
        for(int i = 0; i < MAX_BONE_INFLUENCE; i++) {
            os << v.m_BoneIDs[i];
            if(i != MAX_BONE_INFLUENCE-1) os << ", ";
            else os << "] ";
        }
        os << "boneWeight : [";
        for(int i = 0; i < MAX_BONE_INFLUENCE; i++) {
            os << v.m_Weights[i];
            if(i != MAX_BONE_INFLUENCE-1) os << ", ";
            else os << "] ";
        }
        return os;
    }
}
namespace Components {
    class Mesh : public Renderer {
    public:
        std::vector<Engine::Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Asset::Texture*> textures;
        unsigned int VAO;

        explicit Mesh(const std::vector<Engine::Vertex> &vertices = {},
                      const std::vector<unsigned int> &indices = {},
                      const std::vector<Asset::Texture*> &textures = {},
                      bool use_tangent = false,
                      bool use_skinning = false);

        void setupMesh(const std::vector<Engine::Vertex> &vertices,
                       const std::vector<unsigned int> &indices,
                       const std::vector<Asset::Texture*> &textures,
                      bool use_tangent = false,
                      bool use_skinning = false);
        void SetSkinning(bool use_skinning);
        void SetTangent(bool use_tangent);
        void updateMeshes() const;
        void render() override;
        const char *getComponentName() const override;
        friend std::ostream& operator<<(std::ostream& os, const Mesh& mesh);

    private:
        unsigned int VBO, EBO;
        bool use_tangent;
        bool use_skinning;

        void init() override;
    };

}


#endif