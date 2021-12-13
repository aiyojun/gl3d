#pragma once

namespace glm {

void println(const glm::vec3& v);
void println(const glm::mat4& m);

}

#if defined(ALL_IMPL)

#include <iostream>

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