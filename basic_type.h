#pragma once

#include <string>
#include <utility>
#include <vector>

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    typedef glm::vec3 v3;
    Vertex():
    position(v3(0.0f)), normal(v3(0.0f)), texCoords(v3(0.0f)),
    tangent(v3(0.0f)), bitangent(v3(0.0f)) {}
    Vertex(v3 p, v3 n, v3 tc, v3 t, v3 bt)
    : position(p), normal(n), texCoords(tc), tangent(t), bitangent(bt) {}
};

struct Texture {
    unsigned int id;
    std::string name;
    std::string path;
    Texture()
    : id(0) {}
    Texture(unsigned int id, std::string name, std::string path)
    : id(id), name(std::move(name)), path(std::move(path)) {}
};

struct Material {
    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    Texture Td;
    Texture Ts;
    float shininess;
};


struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    Material material;
    Mesh(const std::vector<Vertex>& v, const std::vector<unsigned int>& i, const std::vector<Texture>& t, const Material& m)
    {vertices = v; indices = i; textures = t; material = m;}
};

class Camera {
public:
//    glm::vec3 rotate;
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 cap;
    glm::mat4 view;
    glm::mat4 projection;
//    glm::vec3 moving;
    float w_h;
    float _near = 0.1f;
    float _far = 100.0f;
    float _degree = 60;
};