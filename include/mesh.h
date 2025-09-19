#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include "shader.h"
#include "texture.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

#define MAX_BONE_INFLUENCE 4
struct Transform {
    glm::vec3 translation;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 getMatrix() const {
        auto m = glm::mat4(1.0f);
        m = glm::translate(m, translation);
        m = glm::rotate(m, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        m = glm::rotate(m, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        m = glm::rotate(m, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        m = glm::scale(m, scale);
        return m;
    }
};
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Weights[MAX_BONE_INFLUENCE];
};
class Mesh {
public:
    Transform *parent;
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    Transform transform;

    unsigned int VAO;

    Mesh(const std::vector<Vertex> &vertices = std::vector<Vertex>(),
        const std::vector<unsigned int> &indices = std::vector<unsigned int>(),
        const std::vector<Texture> &textures = std::vector<Texture>(),
        Transform transform = {glm::vec3(0, 0, 0),glm::vec3(0, 0, 0),glm::vec3(1, 1, 1)},
        Transform *parent = nullptr,
        bool use_tangent = false,
        bool use_skinning = false);

    void Draw(const Shader &shader) const;
    void updateMeshes(bool use_tangent = false, bool use_skinning = false);

private:
    unsigned int VBO, EBO;
    bool use_tangent;
    bool use_skinning;
    void setupMesh();
};
#endif