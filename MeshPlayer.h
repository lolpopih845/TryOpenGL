#pragma once
#include <iostream>
#include <vector>
#include "include/Asset/ModelLoader.h"
#include "include/BaseObject/Mesh.h"
#include "include/Engine/ObjectManager.h"

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

struct FrickingSkyBox {
    Asset::Shader shader;
    unsigned int VAO;
    Asset::Texture texture;
};

inline FrickingSkyBox CreateSkyBox(Asset::Shader shader) {
    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
     std::vector<std::string> f = {
        "resources/sky3.jpg",
        "resources/sky3.jpg",
        "resources/sky1.jpg",
        "resources/sky1.jpg",
        "resources/sky2.jpg",
        "resources/sky2.jpg"};
    const Asset::Texture t(f);
    shader.use();
    shader.setInt("skybox", 0);
    return {shader,skyboxVAO,t};
}

inline void UpdateSkyBox(FrickingSkyBox f, const Camera &camera) {
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    f.shader.use();
    f.shader.setMat4("view", glm::mat4(glm::mat3(camera.GetViewMatrix())));
    // skybox cube
    glBindVertexArray(f.VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, f.texture.ID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}

