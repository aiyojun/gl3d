#pragma once

namespace glm {

    glm::vec3 transform_vertex(const glm::mat4& m, const glm::vec3& n);
    glm::vec3 transform_normal(const glm::mat4& m, const glm::vec3& n);
    float distance_pl(glm::vec3 o, glm::vec3 p, glm::vec3 q);

    void println(const glm::vec3& v);
    void println(const glm::mat4& m);

}

#if defined(ALL_IMPL)

#include <iostream>

inline float glm::distance_pl(glm::vec3 o, glm::vec3 p, glm::vec3 q)
{
    return glm::dot(glm::normalize(p - o), glm::normalize(q - o)) * glm::distance(o, p);
}

inline glm::vec3 glm::transform_vertex(const glm::mat4& m, const glm::vec3& n)
{
    return {m * glm::vec4(n, 1.0)};
}

inline glm::vec3 glm::transform_normal(const glm::mat4& m, const glm::vec3& n)
{
    return glm::normalize(glm::vec3(m * glm::vec4(n, 1.0)) - glm::vec3(m * glm::vec4(glm::vec3(0.f), 1.f)));
}

void glm::println(const glm::vec3& v)
{
    std::cout.setf(std::ios::fixed);
    std::cout << "[ "
    << std::setprecision(4) << v.x << ", "
    << std::setprecision(4) << v.y << ", "
    << std::setprecision(4) << v.z << " ]\n"
    ;
}

void glm::println(const glm::mat4& m)
{
    std::cout.setf(std::ios::fixed);
    std::cout << "[ " 
    << std::setprecision(4) << m[0][0] << ", "
    << std::setprecision(4) << m[1][0] << ", "
    << std::setprecision(4) << m[2][0] << ", "
    << std::setprecision(4) << m[3][0] << ", \n  "
    << std::setprecision(4) << m[0][1] << ", "
    << std::setprecision(4) << m[1][1] << ", "
    << std::setprecision(4) << m[2][1] << ", "
    << std::setprecision(4) << m[3][1] << ", \n  "
    << std::setprecision(4) << m[0][2] << ", "
    << std::setprecision(4) << m[1][2] << ", "
    << std::setprecision(4) << m[2][2] << ", "
    << std::setprecision(4) << m[3][2] << ", \n  "
    << std::setprecision(4) << m[0][3] << ", "
    << std::setprecision(4) << m[1][3] << ", "
    << std::setprecision(4) << m[2][3] << ", "
    << std::setprecision(4) << m[3][3] << " ]\n"
    ;
}

#endif