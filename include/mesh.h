#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include "shader.h"
#include "texture.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

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
    Transform getGlobalTransform(Transform parent) const {
        return Transform({ glm::quat(glm::radians(parent.rotation)) * (parent.scale * translation) + parent.translation, glm::degrees(glm::eulerAngles(glm::quat(glm::radians(parent.rotation))*glm::quat(glm::radians(rotation)))),parent.scale * scale});
    }
};

constexpr Transform DEFAULT_TRANSFORM({glm::vec3(0.0f, 0.0f, 0.f), glm::vec3(.0f, 0.0f, 0.0f),glm::vec3(1.0f,1.0f,1.0f)});
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
    Mesh *parent;
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    Transform transform;

    unsigned int VAO;

    Mesh(const std::vector<Vertex> &vertices = std::vector<Vertex>(),
        const std::vector<unsigned int> &indices = std::vector<unsigned int>(),
        const std::vector<Texture> &textures = std::vector<Texture>(),
        const Transform &transform = {glm::vec3(0, 0, 0),glm::vec3(0, 0, 0),glm::vec3(1, 1, 1)},
        Mesh *parent = nullptr,
        bool use_tangent = false,
        bool use_skinning = false);

    void Draw(const Shader &shader) const;
    void updateMeshes(bool use_tangent = false, bool use_skinning = false);
    Transform getGlobalTransform() const;


private:
    unsigned int VBO, EBO;
    bool use_tangent;
    bool use_skinning;
    void setupMesh();
};

#endif