#pragma once
#include <iostream>
#include <vector>
#include "include/Asset/ModelLoader.h"

inline void CreatingSphere(Components::Mesh &mesh, const int progress) {
    //Stat
    float radius = 1;
    unsigned int sectorCount = 50;
    unsigned int stackCount = 25;

    //Delete vert
    std::vector<Engine::Vertex>().swap(mesh.vertices);

    //----------------------- Vertice -----------------------------------------
    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    //PosStepCal
    float sectorStep = 2 * glm::pi<float>() / sectorCount;
    float stackStep = glm::pi<float>() / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = glm::pi<float>() / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;

            // vertex tex coord (s, t) range between [0, 1]
            s = -(float)j / (sectorCount/2);
            t = (float)i / stackCount;
            mesh.vertices.push_back({glm::vec3(x, y, z), glm::vec3(nx, ny, nz),glm::vec2(s,t)});
        }
    }

    //------------------------------- Indices ---------------------------------------
    std::vector<unsigned int>().swap(mesh.indices);
    int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < (int)((float)sectorCount*progress/100); ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0)
            {
                mesh.indices.push_back(k1);
                mesh.indices.push_back(k2);
                mesh.indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if(i != (stackCount-1))
            {
                mesh.indices.push_back(k1 + 1);
                mesh.indices.push_back(k2);
                mesh.indices.push_back(k2 + 1);
            }
        }
    }
    mesh.updateMeshes();
}

inline void CreatePrism(Components::Mesh &mesh) {
    std::vector<Engine::Vertex>().swap(mesh.vertices);
    float vertex[] = {
        //Front Tri
        0.0f, 0.5f,0.0f    , 0.f,0.f,-1.f  , 0.5f,1.f,
        -0.5f,-0.5f,0.f , 0.f,0.f,-1.f  , 0.f,0.f,
        0.5f,-0.5f,0.f  , 0.f,0.f,-1.f  , 1.f,0.f,
        //Back Tri
        0.0f, 0.5f,1.0f    , 0.f,0.f,1.f   , 0.5f,1.f,
        -0.5f,-0.5f,1.f , 0.f,0.f,1.f   , 0.f,0.f,
        0.5f,-0.5f,1.f  , 0.f,0.f,1.f   , 1.f,0.f,
        //Rec Left
        -0.5f,-0.5f,0.0f, -0.8944f,0.4472f,0,   0.0f,0.0f,
        -0.5f,-0.5f,1.0f, -0.8944f,0.4472f,0,   1.0f,0.0f,
        0.0f, 0.5f,0.0f , -0.8944f,0.4472f,0,   0.0f,1.0f,
        0.0f, 0.5f,1.0f , -0.8944f,0.4472f,0,   1.0f,1.0f,
        //Rec Right
        0.5f,-0.5f,0.0f , 0.8944f,0.4472f,0,    0.0f,0.0f,
        0.5f,-0.5f,1.0f , 0.8944f,0.4472f,0,    1.0f,0.0f,
        0.0f, 0.5f,0.0f , 0.8944f,0.4472f,0,    0.0f,1.0f,
        0.0f, 0.5f,1.0f , 0.8944f,0.4472f,0,    1.0f,1.0f,
        //Rec Down
        -0.5f,-0.5f,0.f , 0.f,-1.f,0.f  , 0.f,0.f,
        0.5f,-0.5f,0.f  , 0.f,-1.f,0.f  , 1.f,0.f,
        -0.5f,-0.5f,1.f , 0.f,-1.f,0.f   , 0.f,1.f,
        0.5f,-0.5f,1.f  , 0.f,-1.f,0.f   , 1.f,1.f,
    };
    for (int i = 0;i < std::size(vertex); i+=8) {
        mesh.vertices.push_back({
            glm::vec3(vertex[i], vertex[i + 1], vertex[i + 2]), glm::vec3(vertex[i + 3], vertex[i + 4], vertex[i + 5]),
            glm::vec2(vertex[i + 6], vertex[i + 7])
        });
    }
    mesh.indices = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        7, 8, 9,
        10,11,13,
        10,12,13,
        14,15,17,
        14,16,17
    };
    mesh.updateMeshes();
}

// class CreateSkyBox {
// public:
//     bool active;
//     Shader shader;
//     Mesh sky;
//
//     explicit Sunny2D(const int id,const Shader &shader): shader(shader) {
//         sky = Mesh();
//         frame = 0;
//         active = true;
//         model = glm::mat4(1.0f);
//         centerT = glm::mat4(1.0f);
//         for (int i = 0; i < 8; i++) rayT[i] = {glm::mat4(1.0f)};
//         sunShader.use();
//         sunShader.setInt("texture1",0);
//     }
//
//     void Draw() {
//         sunShader.use();
//         glBindVertexArray(center.VAO);
//         sunShader.setMat4("model", model*centerT);
//         glDrawElements(GL_TRIANGLES, center.indexCount, GL_UNSIGNED_INT, nullptr);
//         for (int i = 0; i < 8; i++) {
//             glBindVertexArray(ray.VAO);
//             sunShader.setMat4("model", model*rayT[i]);
//             glDrawElements(GL_TRIANGLES, ray.indexCount, GL_UNSIGNED_INT, nullptr);
//         }
//
//     }
//     void Update() {
//         ResetTransform();
//         centerT = glm::scale(centerT, glm::vec3(0.75 + abs(sin((float)glfwGetTime()*2.5))*0.25, 0.75+abs(sin((float)glfwGetTime()*2.5))*0.25, 1.0f));
//         const float radius = 0.7;
//         for (int i=0;i<8;i++) {
//             const float angle = i * glm::two_pi<float>() / 8 + sin(glfwGetTime()*2.25);
//             glm::vec3 pos = glm::vec3(cos(angle), sin(angle), 0.0f) * radius;
//             rayT[i] = glm::translate(rayT[i], pos);
//             const float rotation = angle + glm::half_pi<float>();
//             rayT[i] = glm::rotate(rayT[i], rotation, glm::vec3(0, 0, 1));
//         }
//         model = glm::translate(model, glm::vec3(0, 0, -2));
//         //transform = glm::rotate(transform, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//         Draw();
//     }
//     void ResetTransform() {
//         model = glm::mat4(1.0f);
//         centerT = glm::mat4(1.0f);
//         for(int i=0;i<8;i++) rayT[i] = {glm::mat4(1.0f)};
//     }
//     void Destroy() const {
//         glDeleteVertexArrays(1, &center.VAO);
//         glDeleteBuffers(1, &center.VBO);
//         glDeleteBuffers(1, &center.EBO);
//         glDeleteVertexArrays(1, &ray.VAO);
//         glDeleteBuffers(1, &ray.VBO);
//         glDeleteBuffers(1, &ray.EBO);
//     }
// };