#pragma once

#include "stb_image.h"
#include "basic_type.h"

#include <tuple>
#include <string>
#include <vector>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include <GL/glew.h>

typedef unsigned int index_t;

namespace D3 {
    struct object3u { unsigned int vao, vbo, ebo;};

    enum qualifier { is_1p, is_2c, is_2n, is_3c, is_3t, is_5b };

    template<qualifier Q> struct vertex;
    template<> struct vertex<is_1p> { static object3u allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m); };
    template<> struct vertex<is_2c> { static object3u allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m); };
    template<> struct vertex<is_2n> { static object3u allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m); };
    template<> struct vertex<is_3c> { static object3u allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m); };
    template<> struct vertex<is_3t> { static object3u allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m); };
    template<> struct vertex<is_5b> { static object3u allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m); };

    struct vertex1p : public vertex<is_1p> { glm::vec3 pos; };
    struct vertex2c : public vertex<is_2c> { glm::vec3 pos, color; };
    struct vertex2n : public vertex<is_2n> { glm::vec3 pos, norm; };
    struct vertex3c : public vertex<is_3c> { glm::vec3 pos, norm, color; };
    struct vertex3t : public vertex<is_3t> { glm::vec3 pos, norm; glm::vec2 texCo; };
    struct vertex5b : public vertex<is_5b> { glm::vec3 pos, norm; glm::vec2 texCo; glm::vec3 tan, bitan; };

    struct texture {
        unsigned int id;
        std::string name,  path;
    };

    struct material {
        glm::vec3 Ka, Kd, Ks;
        texture Ta, Td, Ts;
        float shininess;
    };

    struct multi_mesh {
        std::vector<unsigned int> triangles, lines, points;
        std::vector<texture> textures;
        material ma;
        glm::vec3 edge_min, edge_max;
    };

    static std::vector<unsigned int> parse2p(const std::vector<unsigned int>& tri);
    static std::vector<unsigned int> parse2l(const std::vector<unsigned int>& tri);

    template<qualifier T> struct mesh;
    template<> struct mesh<is_1p> : public multi_mesh { std::vector<vertex1p> vertices; };
    template<> struct mesh<is_2c> : public multi_mesh { std::vector<vertex2c> vertices; };
    template<> struct mesh<is_2n> : public multi_mesh { std::vector<vertex2n> vertices; };
    template<> struct mesh<is_3c> : public multi_mesh { std::vector<vertex3c> vertices; };
    template<> struct mesh<is_3t> : public multi_mesh { std::vector<vertex3t> vertices; };
    template<> struct mesh<is_5b> : public multi_mesh { std::vector<vertex5b> vertices; };

    typedef mesh<is_1p> mesh1p;
    typedef mesh<is_2c> mesh2c;
    typedef mesh<is_2n> mesh2n;
    typedef mesh<is_3c> mesh3c;
    typedef mesh<is_3t> mesh3t;
    typedef mesh<is_5b> mesh5b;

    class AI {
    public:
        static material decompose1m(const aiScene* scene, const aiMesh* ai_mesh);
        static mesh1p decompose1p(const aiScene* scene, const aiMesh* ai_mesh);
        static mesh2c decompose2c(const aiScene* scene, const aiMesh* ai_mesh);
        static mesh2n decompose2n(const aiScene* scene, const aiMesh* ai_mesh);
        static mesh3c decompose3c(const aiScene* scene, const aiMesh* ai_mesh);
        static mesh3t decompose3t(const aiScene* scene, const aiMesh* ai_mesh);
        static mesh5b decompose5b(const aiScene* scene, const aiMesh* ai_mesh);
        static void iterate1p(const aiScene* scene, const aiNode* node, std::vector<mesh1p>& meshes, bool m);
        static void iterate2c(const aiScene* scene, const aiNode* node, std::vector<mesh2c>& meshes, bool m);
        static void iterate2n(const aiScene* scene, const aiNode* node, std::vector<mesh2n>& meshes, bool m);
        static void iterate3c(const aiScene* scene, const aiNode* node, std::vector<mesh3c>& meshes, bool m);
        static void iterate3t(const aiScene* scene, const aiNode* node, std::vector<mesh3t>& meshes, bool m);
        static void iterate5b(const aiScene* scene, const aiNode* node, std::vector<mesh5b>& meshes, bool m);
    };

    struct three1p {
        void prepare();
        void render();
        std::vector<mesh1p> meshes;
        std::vector<std::tuple<object3u, object3u, object3u>> objects;
    };

    struct three2c {
        void prepare();
        void render();
        std::vector<mesh2c> meshes;
        std::vector<std::tuple<object3u, object3u, object3u>> objects;
    };

    struct three2n {
        void prepare();
        void render();
        std::vector<mesh2n> meshes;
        std::vector<std::tuple<object3u, object3u, object3u>> objects;
    };

    struct three3c {
        void prepare();
        void render();
        std::vector<mesh3c> meshes;
        std::vector<std::tuple<object3u, object3u, object3u>> objects;
    };

    struct three3t {
        void prepare();
        void render();
        std::vector<mesh3t> meshes;
        std::vector<std::tuple<object3u, object3u, object3u>> objects;
    };

    struct three5b {
        void prepare();
        void render();
        std::vector<mesh5b> meshes;
        std::vector<std::tuple<object3u, object3u, object3u>> objects;
    };

    class three3d {
    public:
        static three1p load1p(const std::string& path, bool material_open = false);
        static three2c load2c(const std::string& path, bool material_open = false);
        static three2n load2n(const std::string& path, bool material_open = false);
        static three3c load3c(const std::string& path, bool material_open = false);
        static three3t load3t(const std::string& path, bool material_open = false);
        static three5b load5b(const std::string& path, bool material_open = false);
    };
}