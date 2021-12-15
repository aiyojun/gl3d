#pragma once

#include <map>
#include <set>
#include <tuple>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

typedef unsigned int index_t;

namespace D3 {
    struct object3u { unsigned int vao, vbo, ebo;};

    struct vertex1p { 
        glm::vec3 pos; 
        static object3u allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m);
    };
    struct vertex2c { 
        glm::vec3 pos, color; 
        static object3u allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m);
    };
    struct vertex2n { 
        glm::vec3 pos, norm; 
        static object3u allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m);
    };
    struct vertex3c { 
        glm::vec3 pos, norm, color; 
        static object3u allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m);
    };
    struct vertex3t { 
        glm::vec3 pos, norm; glm::vec2 texCo; 
        static object3u allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m);
    };
    struct vertex5b { 
        glm::vec3 pos, norm; glm::vec2 texCo; glm::vec3 tan, bitan; 
        static object3u allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m);
    };

    struct texture {
        unsigned int id;
        std::string name, path;
    };

    struct material {
        glm::vec3 Ka, Kd, Ks;
        texture Ta, Td, Ts;
        float shininess;
    };

    struct multi_mesh {
        std::vector<unsigned int> triangles, lines, points;
        material ma;
        glm::vec3 edge_min, edge_max;
    };

    static std::vector<unsigned int> parse2p(const std::vector<unsigned int>& tri);
    static std::vector<unsigned int> parse2l(const std::vector<unsigned int>& tri);

    struct mesh1p : public multi_mesh { std::vector<vertex1p> vertices; };
    struct mesh2c : public multi_mesh { std::vector<vertex2c> vertices; };
    struct mesh2n : public multi_mesh { std::vector<vertex2n> vertices; };
    struct mesh3c : public multi_mesh { std::vector<vertex3c> vertices; };
    struct mesh3t : public multi_mesh { std::vector<vertex3t> vertices; };
    struct mesh5b : public multi_mesh { std::vector<vertex5b> vertices; };


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

    unsigned int loadTexture(const std::string& path);

    typedef void (*shader_func)(material* m);

    struct with_shader {
        shader_func pre_ptr = nullptr;
        shader_func pos_ptr = nullptr;
    };

    struct three1p {
        void prepare();
        void render();
        void shader_set(const shader_func& pre, const shader_func& pos);
        with_shader ws;
        bool material_open;
        std::vector<mesh1p> meshes;
        std::vector<std::tuple<object3u, object3u, object3u>> objects;
    };

    struct three2c {
        void prepare();
        void render();
        void shader_set(const shader_func& pre, const shader_func& pos);
        with_shader ws;
        bool material_open;
        std::vector<mesh2c> meshes;
        std::vector<std::tuple<object3u, object3u, object3u>> objects;
    };

    struct three2n {
        void prepare();
        void render();
        void shader_set(const shader_func& pre, const shader_func& pos);
        with_shader ws;
        bool material_open;
        std::vector<mesh2n> meshes;
        std::vector<std::tuple<object3u, object3u, object3u>> objects;
    };

    struct three3c {
        void prepare();
        void render();
        void shader_set(const shader_func& pre, const shader_func& pos);
        with_shader ws;
        bool material_open;
        std::vector<mesh3c> meshes;
        std::vector<std::tuple<object3u, object3u, object3u>> objects;
    };

    struct three3t {
        void prepare();
        void render();
        void shader_set(const shader_func& pre, const shader_func& pos);
        with_shader ws;
        bool material_open;
        std::vector<mesh3t> meshes;
        std::vector<std::tuple<object3u, object3u, object3u>> objects;
    };

    struct three5b {
        void prepare();
        void render();
        void shader_set(const shader_func& pre, const shader_func& pos);
        with_shader ws;
        bool material_open;
        std::vector<mesh5b> meshes;
        std::vector<std::tuple<object3u, object3u, object3u>> objects;
    };

    static three1p load1p(const std::string& path, bool material_open = false);
    static three2c load2c(const std::string& path, bool material_open = false);
    static three2n load2n(const std::string& path, bool material_open = false);
    static three3c load3c(const std::string& path, bool material_open = false);
    static three3t load3t(const std::string& path, bool material_open = false);
    static three5b load5b(const std::string& path, bool material_open = false);
}