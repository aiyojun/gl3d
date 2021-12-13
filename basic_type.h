#pragma once

#include <string>
#include <utility>
#include <vector>

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    // glm::vec2 texCoords;
//    glm::vec3 tangent;
//    glm::vec3 bitangent;
    typedef glm::vec3 v3;
    Vertex():
    position(v3(0.0f)), normal(v3(0.0f)) {}
	//position(v3(0.0f)), normal(v3(0.0f)), texCoords(v3(0.0f)) {}
//    tangent(v3(0.0f)), bitangent(v3(0.0f)) {}
//    Vertex(v3 p, v3 n, v3 tc, v3 t, v3 bt)
//    : position(p), normal(n), texCoords(tc), tangent(t), bitangent(bt) {}
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
    glm::vec3 Ka = glm::vec3(0.f);
    glm::vec3 Kd = glm::vec3(0.f);
    glm::vec3 Ks = glm::vec3(0.f);
    Texture Ta;
    Texture Td;
    Texture Ts;
    float shininess = 0.f;
    Material(): Ka(0.0f), Kd(0.0f), Ks(0.0f), Ta(), Td(), Ts(), shininess(0) {}
};


struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    Material material;
    Mesh(std::vector<Vertex> v, std::vector<unsigned int> i)
    : vertices(std::move(v)), indices(std::move(i)) {}
    Mesh(std::vector<Vertex> v, std::vector<unsigned int> i, Material m)
    : vertices(std::move(v)), indices(std::move(i)), material(std::move(m)) {}
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

struct Light {
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct ray_t {
    float ia, id, is;
    glm::vec3 dir, pos, color;
};

struct vertex_t {
    glm::vec3 pos, norm, color;
    glm::vec2 tex;
};

struct mesh_t {
    std::vector<vertex_t> vertices;
    std::vector<unsigned int> indices;
//    std::vector<unsigned int> indices_lines;
//    std::vector<unsigned int> indices_points;
//    std::vector<unsigned int> indices_triangles;
    std::vector<unsigned int> indices_point();
    std::vector<unsigned int> indices_lines();
};

#if defined(ALL_IMPL)
#include <map>
#include <tuple>

static void cache_append(
        std::map<std::string, std::tuple<unsigned int, unsigned int>>& cache,
        unsigned int a, unsigned int b)
{
    std::string uid = (a <= b)
            ? (std::to_string(a) + "-" + std::to_string(b))
            : (std::to_string(b) + "-" + std::to_string(a))
            ;
    if (cache.find(uid) == cache.end())
        cache[uid] = std::move(std::tuple<unsigned int, unsigned int>(a, b));
}

std::vector<unsigned int> mesh_t::indices_point()
{
    std::map<std::string, std::tuple<unsigned int, unsigned int>> cache;
    unsigned int index = 0;
    while (index + 3 < indices.size()) {
        cache_append(cache, indices[index    ], indices[index + 1]);
        cache_append(cache, indices[index    ], indices[index + 2]);
        cache_append(cache, indices[index + 1], indices[index + 2]);
        index = index + 3;
    }
    return {};
}
std::vector<unsigned int> mesh_t::indices_lines()
{
    return {};
}
#endif