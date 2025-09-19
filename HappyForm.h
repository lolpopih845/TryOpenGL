#pragma once
#include "MeshPlayer.h"
#include "include/mesh.h"

class Merlin {
public:
    Shader shader;
    Transform transform;
    Mesh center;
    Mesh handLeft;
    Mesh handRight;
    unsigned int state;
    int centerProg;

    Merlin(Shader shader, const Transform &transform): shader(shader) {
        state = 0;
        this->transform = transform;
        center = Mesh();
        CreatingSphere(center, 0);
        centerProg = 0;
    }

    void Draw() {
        switch (state) {
            case 0:
               return;
            case 1:
                center.Draw(shader);


        }
    }
    void Update(float time) {
        state = std::max(3,(int)time/5);
        switch (state) {
            case 0:
                return;
            case 1:
                if (centerProg<100) CreatingSphere(center, ++centerProg);
                break;
            case 2:
                break;
            case 3:
                break;
            default:
                break;
        }
    }

private:


};

class Rotater {
public:

private:

};