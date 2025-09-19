#pragma once
#include <iostream>
#include <vector>
#include "include/mesh.h"

struct Mesh2D {
    unsigned int VAO,VBO, EBO;
    size_t indexCount;
};

Mesh2D Create2DSun() {
    unsigned int VBO, VAO, EBO;
    constexpr int numVertices = 64;
    float circleRadius = 0.5f;

    std::vector<float> circleVertices;
    std::vector<unsigned int> circleIndices;

    circleVertices.push_back(0.0f); // x
    circleVertices.push_back(0.0f); // y
    circleVertices.push_back(1.0f); // z
    circleVertices.push_back(0.5f); // u
    circleVertices.push_back(0.5f); // v
    for (int i = 0; i < numVertices; i++) {
        float angle = 2.0f * M_PI * i / numVertices;
        float x = circleRadius * cos(angle);
        float y = circleRadius * sin(angle);
        float u = 0.5f + (cos(angle) * 0.5f);
        float v = 0.5f + (sin(angle) * 0.5f);
        circleVertices.push_back(x);
        circleVertices.push_back(y);
        circleVertices.push_back(1);
        circleVertices.push_back(u); // u
        circleVertices.push_back(v); // v
        circleIndices.push_back(0);              // Center
        circleIndices.push_back(i + 1);          // Current edge
        circleIndices.push_back((i + 1) % numVertices + 1);
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, circleIndices.size() * sizeof(unsigned int), circleIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return {VAO, VBO, EBO, circleIndices.size()};
}
Mesh2D Create2DRay() {
    unsigned int VBO, VAO, EBO;
    constexpr int numRay = 16;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    float squareVertices[20] = {
        0.7f,0.7f,1, 0.,0.,
        0.9f,0.7f,1,1.,0.,
        0.9f,0.9f,1,1.,1.,
        0.7f,0.9f,1,0.,1.
    };
    int indices[6] = {0, 1, 2, 2, 3, 0};
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return {VAO, VBO, EBO, 6};
}

    class Sunny2D {
        public:
            bool active;
            int sunID;
            Shader sunShader;
            unsigned int frame;
            Mesh2D center;
            Mesh2D ray;
            glm::mat4 model;
            glm::mat4 centerT;
            glm::mat4 rayT[8];

            explicit Sunny2D(const int id,const Shader &shader): sunShader(shader) {
                sunID = id;
                ray = Create2DRay();
                center = Create2DSun();

                frame = 0;
                active = true;
                model = glm::mat4(1.0f);
                centerT = glm::mat4(1.0f);
                for (int i = 0; i < 8; i++) rayT[i] = {glm::mat4(1.0f)};
                sunShader.use();
                sunShader.setInt("texture1",0);
            }

            void Draw() {
                sunShader.use();
                glBindVertexArray(center.VAO);
                sunShader.setMat4("model", model*centerT);
                glDrawElements(GL_TRIANGLES, center.indexCount, GL_UNSIGNED_INT, nullptr);
                for (int i = 0; i < 8; i++) {
                    glBindVertexArray(ray.VAO);
                    sunShader.setMat4("model", model*rayT[i]);
                    glDrawElements(GL_TRIANGLES, ray.indexCount, GL_UNSIGNED_INT, nullptr);
                }

            }
            void Update() {
                ResetTransform();
                centerT = glm::scale(centerT, glm::vec3(0.75 + abs(sin((float)glfwGetTime()*2.5))*0.25, 0.75+abs(sin((float)glfwGetTime()*2.5))*0.25, 1.0f));
                const float radius = 0.7;
                for (int i=0;i<8;i++) {
                    const float angle = i * glm::two_pi<float>() / 8 + sin(glfwGetTime()*2.25);
                    glm::vec3 pos = glm::vec3(cos(angle), sin(angle), 0.0f) * radius;
                    rayT[i] = glm::translate(rayT[i], pos);
                    const float rotation = angle + glm::half_pi<float>();
                    rayT[i] = glm::rotate(rayT[i], rotation, glm::vec3(0, 0, 1));
                }
                model = glm::translate(model, glm::vec3(0, 0, -5));
                //transform = glm::rotate(transform, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                Draw();
            }
            void ResetTransform() {
                model = glm::mat4(1.0f);
                centerT = glm::mat4(1.0f);
                for(int i=0;i<8;i++) rayT[i] = {glm::mat4(1.0f)};
            }
            void Destroy() const {
                glDeleteVertexArrays(1, &center.VAO);
                glDeleteBuffers(1, &center.VBO);
                glDeleteBuffers(1, &center.EBO);
                glDeleteVertexArrays(1, &ray.VAO);
                glDeleteBuffers(1, &ray.VBO);
                glDeleteBuffers(1, &ray.EBO);
            }
        };

void CreatingSphere(Mesh &mesh, int progress) {
    //Stat
    float radius = 1;
    unsigned int sectorCount = 50;
    unsigned int stackCount = 25;

    //Delete vert
    std::vector<Vertex>().swap(mesh.vertices);

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

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
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