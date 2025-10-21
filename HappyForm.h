#pragma once
#include <GLFW/glfw3.h>
#include "MeshPlayer.h"
#include "include/BaseObject/Mesh.h"
#include "include/Engine/ObjectManager.h"


namespace Components {
    class Rotater : public Component {
    public:
        Engine::Transform orbitRing; //Pos(x,y,z), Rot(x) -> 2D invisible circular plane, Slc(x,y) oral plane
        Engine::GameObject* prisms[4];
        unsigned int currentIndex = 0;
        Asset::Shader& shader;
        float delay;
        float spawnTime = 0;
        float frameTime = 0;
        float r_speed = 1.0f;
        Rotater(Asset::Shader& shader, Engine::Transform orbitRing, float delay, float r_speed)
            : orbitRing(orbitRing), shader(shader), delay(delay), r_speed(r_speed) {
        }
        void init() override {
            for (int i = 0; i < 4; ++i) {
                prisms[i] = Engine::ObjectManager::CreateObject<Prefab::MeshObject>();
                prisms[i]->getComponent<Mesh>()->shader = &shader;
                gameObject->addChild(prisms[i]);
            }
        }
        void update(float dTime) override {
            frameTime += dTime;
            if (delay > 0) {
                delay -= dTime;
                return;
            }
            spawnTime -= dTime;
            if (currentIndex < 4 && spawnTime<0) {
                spawnTime = 0.5f;
                CreatePrism(*prisms[currentIndex]->getComponent<Mesh>());

                currentIndex++;
            }

            orbitRing.rotation.x += r_speed;

            for (int i = 0; i < 4; ++i) {
                CalMatrix(prisms[i]->getComponent<Transform>(), frameTime + i*0.33f);
            }
        }
    private:
        void CalMatrix(Transform* t,float time) {
            float omega = 5.0f;
            float theta = time * omega;
            glm::vec3 C = orbitRing.translation;
            float rx = orbitRing.scale[0], ry = orbitRing.scale[1];

            auto p_local = glm::vec3(rx * cos(theta), ry * sin(theta), 0.0f);
            glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(orbitRing.rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
            glm::vec3 P = C + glm::vec3(R * glm::vec4(p_local, 1.0f));

            glm::vec3 t_local = glm::vec3(-rx * sin(theta), ry * cos(theta), 0.0f);
            glm::vec3 forward = glm::normalize(glm::vec3(R * glm::vec4(t_local, 0.0f)));

            float yaw = glm::degrees(atan2(forward.y, forward.x));
            float pitch = glm::degrees(asin(forward.z));
            t->setTransform(Engine::Transform({P, glm::vec3(pitch,yaw+90,0),glm::vec3(0.5f)}));
        }
    };
}

inline Engine::GameObject* CreateRotater(Asset::Shader& shader, Engine::Transform orbitRing, float delay, float r_speed) {
    Engine::GameObject* obj = Engine::ObjectManager::CreateObject<Engine::GameObject>();
    obj->addComponent<Components::Rotater>(shader, orbitRing, delay, r_speed);
    return obj;
}

class Hand : public Engine::GameObject {

};


namespace Components {
    class Merlin : public Component {
    public:
        float time = 0;
        unsigned int state = 0;
        float centerProg = 0;
        int last_state = 0;
        Asset::Shader* shader;
        Transform *transform;
        Mesh* center;
        //Hand* hands1;
        //Hand* hands2;
        Engine::GameObject* rotater1;
        Engine::GameObject* rotater2;
        void init() override {
            shader = gameObject->getComponent<Mesh>()->shader;
            center = gameObject->getComponent<Mesh>();
            CreatingSphere(*center, 0);
            transform = gameObject->getComponent<Transform>();
            transform->setTransform(Engine::Transform({{0,0,-1},glm::vec3(90,0,0),glm::vec3(1)}));
        }

        void update(float dTime) override {
            time+=dTime;
            state = std::min(2,(int)time/3);
            if (state != last_state && state == 2) {
                last_state = state;
                rotater1 = CreateRotater(*shader,Engine::Transform({glm::vec3(0,0,0),{30,0,0},{1.5,1,0}}),0,0.02f);
                rotater2 = CreateRotater(*shader,Engine::Transform({glm::vec3(0,0,0),{120,0,0},{2,1.5,0}}),3,0.01f);
                gameObject->addChild(rotater1);
                gameObject->addChild(rotater2);
            }
            switch (state) {
                case 0:
                    return;
                case 1:
                    if (centerProg<100) {
                        centerProg += dTime*50;
                        CreatingSphere(*center, (int)centerProg);
                    }
                    break;
                case 2:
                    transform->setTransform(Engine::Transform({{0,sin(time)/2,-1},glm::vec3(90,0,time*250),glm::vec3(1)}));
                    //for (int i=0;i<rotaters.size();i++) rotaters[i].Update(time);
                    break;
                default:
                    break;
            }
        }
    };
}

inline Engine::GameObject* CreateMerlin(Asset::Shader* shader) {
    Engine::GameObject* obj = Engine::ObjectManager::CreateObject<Engine::GameObject>();
    obj->addComponent<Components::Mesh>();
    obj->getComponent<Components::Mesh>()->shader = shader;
    obj->addComponent<Components::Merlin>();
    return obj;
}

