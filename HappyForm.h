#pragma once
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>

#include "MeshPlayer.h"

class Rotater {
public:
    Transform orbitRing; //Pos(x,y,z), Rot(x) -> 2D invisible circular plane, Slc(x,y) oral plane
    Transform parent;
    Mesh prisms[4] = {Mesh(),Mesh(),Mesh(),Mesh()};
    unsigned int currentIndex = 0;
    Shader shader;
    float delay;
    float lastTime;
    float r_speed;

    Rotater(const Shader shader, const Transform &orbitRing, const Transform &parent, float delay, float r_speed): orbitRing(orbitRing),parent(parent), shader(shader),delay(delay),r_speed(r_speed) {
        lastTime = glfwGetTime();
    }
    void Draw() const {
        for (const auto &prism: prisms) {
            prism.Draw(shader);
        }
    }
    void Update(const float time) {
        if (delay>0) {
            if (time - lastTime > delay) delay = 0;
            else return;
        }
        if (currentIndex<4 && time-lastTime > 0.5f) {
            lastTime = time;
            CreatePrism(prisms[currentIndex]);
            currentIndex++;
        }

        orbitRing.rotation[0] += r_speed;
        Transform new_parent = Transform({{0,sin(time)/5,-1.2},glm::vec3(0),glm::vec3(1)}).getGlobalTransform(parent);
        for (int i=0;i<4;i++) {
            //prisms[i].transform = DEFAULT_TRANSFORM.getGlobalTransform(parent);
            prisms[i].transform = CalMatrix(time+i*0.33).getGlobalTransform(new_parent);
        }
    }
private:
    Transform CalMatrix(float time) {
        float omega = 5.0f;
        float theta = time * omega;
        glm::vec3 C = glm::vec3(orbitRing.translation[0], orbitRing.translation[1], orbitRing.translation[2]);
        float rx = orbitRing.scale[0], ry = orbitRing.scale[1];
        auto p_local = glm::vec3(rx * cos(theta), ry * sin(theta), 0.0f);
        glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(orbitRing.rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::vec3 P = C + glm::vec3(R * glm::vec4(p_local, 1.0f));
        glm::vec3 t_local = glm::vec3(-rx * sin(theta), ry * cos(theta), 0.0f);
        glm::vec3 t_world = glm::vec3(R * glm::vec4(t_local, 0.0f));
        glm::vec3 forward = glm::normalize(t_world);
        float yaw = glm::degrees(atan2(forward.y, forward.x));
        float pitch = glm::degrees(asin(forward.z));
        return Transform({P, glm::vec3(pitch,yaw+90,0),{0.2,0.2,0.2f}});
    }
};
class Merlin {
public:
    float time;
    Shader shader;
    Transform transform;
    Mesh center;
    unsigned int state;
    float centerProg;
    int last_state;
    std::vector<Rotater> rotaters;

    Merlin(Shader shader,Transform transform): shader(shader),transform(transform) {
        time = 0;
        state = 0;
        center = Mesh();
        CreatingSphere(center, 0);
        center.transform = Transform({{0,0,-1},glm::vec3(90,0,0),glm::vec3(0.5)}).getGlobalTransform(transform);
        centerProg = 0;
        last_state = 0;
        rotaters.push_back(Rotater(shader,{glm::vec3(0,0,0),{30,0,0},{1,0.5,0}},transform,0,0.02f));
        rotaters.push_back(Rotater(shader,{glm::vec3(0,0,0),{120,0,0},{1.25,0.75,0}},transform,3,0.01f));
    }

    void Draw() const {
        switch (state) {
            case 0:
               return;
            case 2:
                for (int i=0;i<rotaters.size();i++) rotaters[i].Draw();
            case 1:
                center.Draw(shader);
                break;
           default:
                return;
        }
    }
    void Update(const float &dtime) {
        time+=dtime;
        state = std::min(2,(int)time/3);
        if (state != last_state && state == 2) {
            last_state = state;
            for (int i=0;i<rotaters.size();i++) rotaters[i].lastTime = time;
        }
        switch (state) {
            case 0:
                return;
            case 1:
                if (centerProg<100) {
                    centerProg += dtime*50;
                    CreatingSphere(center, (int)centerProg);
                }
                break;
            case 2:
                center.transform = Transform({{0,sin(time)/5,-1},glm::vec3(90,0,time*250),glm::vec3(0.5)}).getGlobalTransform(transform);
                for (int i=0;i<rotaters.size();i++) rotaters[i].Update(time);
                break;
            default:
                break;
        }
    }

private:


};

