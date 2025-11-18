#pragma once
#include <GLFW/glfw3.h>
#include "MeshPlayer.h"
#include "include/Asset/AssetStorage.h"
#include "include/BaseObject/Mesh.h"
#include "include/Engine/ObjectManager.h"
#include "include/Asset/ModelLoader.h"
#include "include/Components/animator.h"
#include "include/Components/Collider.h"
inline Engine::GameObject* CreateModelObject(const std::string& name, const std::string& path) {
    Engine::GameObject* obj =  Asset::ModelLoader::LoadModelGameObject(name,path);
    return obj;
}

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
                prisms[i] = Engine::ObjectManager::CreateObject<Prefab::MeshObject>("Rotater");
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
    Engine::GameObject* obj = Engine::ObjectManager::CreateObject<Engine::GameObject>("Rotater");
    obj->addComponent<Components::Rotater>(shader, orbitRing, delay, r_speed);
    return obj;
}

namespace Components {
    class Merlin : public Component {
    public:
        float gameTime = 0;
        float time = 0;
        unsigned int state = 0;
        float centerProg = 0;
        int last_state = 0;
        int hp = 3;
        Asset::Shader* shader;
        Transform *transform;
        Mesh* center;
        Engine::GameObject* target;
        Engine::GameObject* rotater1;
        Engine::GameObject* rotater2;
        void init() override {
            shader = gameObject->getComponent<Mesh>()->shader;
            center = gameObject->getComponent<Mesh>();
            CreatingSphere(*center, 0);
            transform = gameObject->getComponent<Transform>();
        }

        void update(float dTime) override {
            time+=dTime;
            state = std::min(2,(int)time/3);
            if (hp<=0) Engine::ObjectManager::DestroyObject(gameObject);
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
                    transform->setPosition(transform->getTransform().translation+glm::normalize(target->getComponent<Transform>()->getGlobalTransform().translation - transform->getGlobalTransform().translation)*dTime*(2.f+gameTime*0.05f));
                    transform->setRotation(glm::vec3(90,0,time*250));
                    break;
                default:
                    break;
            }
        }
    };
}

namespace Components {
    class Note : public Component {
    public:
        Transform* transform;
        Collider* collider;
        glm::vec3 velocity;
        float lifeTime = 5;
        void init() override {
            transform = gameObject->getComponent<Transform>();
            collider = gameObject->getComponent<Collider>();
        }
        void update(float dTime) override {
            transform->setPosition(transform->getGlobalTransform().translation + velocity*dTime*20.f);
            transform->setRotation(transform->getGlobalTransform().rotation + glm::vec3(0, dTime*360, 0));
            lifeTime -= dTime;
            if (lifeTime < 0) Engine::ObjectManager::DestroyObject(gameObject);
        }
        void onCollisionEnter(Collider *other) override {
            if (other->gameObject->name == "merlin") {
                other->gameObject->getComponent<Merlin>()->hp-=1;
                Engine::ObjectManager::DestroyObject(gameObject);
            }
        }
    };
    class MixamoBoy : public Component {
    public:
        Transform* transform;
        Animator* animator;
        Camera* camera;
        glm::vec3 accel = {0,0,0};
        glm::vec3 velo = {0,0,0};
        bool isWalking = false;
        void init() override {
            transform = gameObject->getComponent<Transform>();
            animator = gameObject->getComponent<Animator>();
            animator->SetSkeleton(Asset::AssetStorage<Asset::Skeleton>::Get("mixamoBoy"));
            animator->PlayAnimation(Asset::AssetStorage<Asset::Animation>::Get("Neutral").get());
        }
        void update(float dTime) override {
            std::cout << *animator << std::endl;
            if (accel[0]<=0)
                velo[0] = glm::max(velo[0]-dTime,0.f);
            else
                velo[0] = glm::min(velo[0]+accel[0]*dTime,5.f);
            if (accel[2]<=0)
                velo[2] = glm::max(velo[2]-dTime,0.f);
            else
                velo[2] = glm::min(velo[2]+accel[2]*dTime,5.f);
            accel[0]=0;
            accel[2]=0;
            if (!isWalking && (velo[0]!=0 || velo[2]!=0)) {
                isWalking = true;
                animator->PlayAnimation(Asset::AssetStorage<Asset::Animation>::Get("Walking").get());
            }
            else if (isWalking && (velo[0]==0 && velo[2]==0)) {
                isWalking = false;
                animator->PlayAnimation(Asset::AssetStorage<Asset::Animation>::Get("Neutral").get());
            }
            transform->setPosition(transform->getGlobalTransform().translation + velo*dTime);
            camera->UpdateCam(this->gameObject);
        }
    };
}
inline Engine::GameObject* CreateNote(glm::vec3 pos, glm::vec3 vel) {
    Engine::GameObject* obj = CreateModelObject("resources/objects/note/note.obj");
    obj->addComponent<Components::Collider>(glm::vec3(0),glm::vec3(0.25));
    obj->addComponent<Components::Note>();
    obj->getComponent<Components::Transform>()->setTransform({pos,glm::vec3(0,0,0),glm::vec3(1)});
    obj->getComponent<Components::Note>()->velocity = vel;
    obj->name = "note";
    return obj;
}
inline Engine::GameObject* CreateMerlin(Asset::Shader* shader,Engine::GameObject* target,float timey) {
    Engine::GameObject* obj = Engine::ObjectManager::CreateObject<Engine::GameObject>("Merlin");
    obj->addComponent<Components::Mesh>();
    obj->getComponent<Components::Mesh>()->shader = shader;
    obj->addComponent<Components::Merlin>();
    obj->getComponent<Components::Merlin>()->gameTime = timey;
    obj->getComponent<Components::Merlin>()->target = target;
    obj->getComponent<Components::Transform>()->setTransform(Engine::Transform({glm::vec3((float)(10 - rand()%20),(float)(5 - rand()%10),(float)(10 - rand()%20))+target->getComponent<Components::Transform>()->getGlobalTransform().translation,glm::vec3(90,0,0),glm::vec3(1)}));
    obj->addComponent<Components::Collider>(glm::vec3(0),glm::vec3(1));
    obj->name = "merlin";
    return obj;
}

namespace Components {
    class Tp : public Component {
        public:
        Transform* transform;
        Camera* camera;
        float trigger=0;
        void init() override {
            transform = gameObject->getComponent<Transform>();
            //
            transform->setScale(glm::vec3(0.075f));
        }
        void update(float dTime) override {
            if (trigger>0) trigger-=dTime*120;
            glm::quat camRot = glm::quat(glm::vec3(0., -glm::radians(camera->Yaw), glm::radians(camera->Pitch)));
            glm::vec3 worldOffset = camRot * glm::vec3(0.15,-0.05,0.1);
            transform->setPosition(camera->Position + worldOffset);
            transform->setRotation(glm::vec3(0,-camera->Yaw,camera->Pitch+trigger));
        }
        void Fire() {
            if (trigger>0) return;
            trigger = 45;
            //Fire Note
            CreateNote(camera->Position,glm::vec3(
                glm::cos(glm::radians(camera->Pitch)) * glm::cos(glm::radians(camera->Yaw)),
                glm::sin(glm::radians(camera->Pitch)),
                glm::cos(glm::radians(camera->Pitch)) * glm::sin(glm::radians(camera->Yaw))
                ));
        }
    };
}

inline Engine::GameObject* CreateTrumpet(Camera* camera) {
    Engine::GameObject* obj = CreateModelObject("resources/objects/doot/doot.obj");
    obj->addComponent<Components::Tp>();
    obj->getComponent<Components::Tp>()->camera = camera;
    return obj;
}

