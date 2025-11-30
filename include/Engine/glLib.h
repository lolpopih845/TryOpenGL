#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

inline glm::mat4 quatToMat4(const glm::quat& q)
{
    float x = q.x;
    float y = q.y;
    float z = q.z;
    float w = q.w;

    glm::mat4 m(1.0f);

    float xx = x * x;
    float yy = y * y;
    float zz = z * z;
    float xy = x * y;
    float xz = x * z;
    float yz = y * z;
    float wx = w * x;
    float wy = w * y;
    float wz = w * z;

    m[0][0] = 1.0f - 2.0f * (yy + zz);
    m[0][1] =        2.0f * (xy + wz);
    m[0][2] =        2.0f * (xz - wy);

    m[1][0] =        2.0f * (xy - wz);
    m[1][1] = 1.0f - 2.0f * (xx + zz);
    m[1][2] =        2.0f * (yz + wx);

    m[2][0] =        2.0f * (xz + wy);
    m[2][1] =        2.0f * (yz - wx);
    m[2][2] = 1.0f - 2.0f * (xx + yy);

    return m;
}