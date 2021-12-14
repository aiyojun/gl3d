#include "D3.h"
#include <map>
#include <set>
#include <tuple>

namespace D3 {

#define GEN_OBJECT                               \
    index_t vao, vbo, ebo;                       \
    glGenVertexArrays(1, &vao);                  \
    glGenBuffers(1, &vbo);                       \
    glGenBuffers(1, &ebo)
#define BIND_VBO(c, p, n)                        \
    glBindVertexArray(vao);                      \
    glBindBuffer(GL_ARRAY_BUFFER, vbo);          \
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) ((n) * sizeof(c)), (p), GL_STATIC_DRAW)
#define BIND_EBO(p, n)                           \
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);  \
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (n) * sizeof(unsigned int), (p), GL_STATIC_DRAW)
#define UNBIND_OBJECT                            \
    glBindVertexArray(0);                        \
    glBindBuffer(GL_ARRAY_BUFFER, 0);            \
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)

    object3u vertex<is_1p>::allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m)
    {
        GEN_OBJECT;
        BIND_VBO(vertex1p, vertices, n);
        BIND_EBO(indices, m);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
        UNBIND_OBJECT;
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
        return {vao, vbo, ebo};
    }
    object3u vertex<is_2c>::allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m)
    {
        GEN_OBJECT;
        BIND_VBO(vertex2c, vertices, n);
        BIND_EBO(indices, m);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));

        UNBIND_OBJECT;
        return {vao, vbo, ebo};
    }
    inline object3u vertex<is_2n>::allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m)
    {
        return vertex<is_2c>::allocate(vertices, n, indices, m);
    }
    object3u vertex<is_3c>::allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m)
    {
        GEN_OBJECT;
        BIND_VBO(vertex3c, vertices, n);
        BIND_EBO(indices, m);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (6 * sizeof(float)));

        UNBIND_OBJECT;
        return {vao, vbo, ebo};
    }
    object3u vertex<is_3t>::allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m)
    {
        GEN_OBJECT;
        BIND_VBO(vertex3t, vertices, n);
        BIND_EBO(indices, m);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));

        UNBIND_OBJECT;
        return {vao, vbo, ebo};
    }
    object3u vertex<is_5b>::allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m)
    {
        GEN_OBJECT;
        BIND_VBO(vertex5b, vertices, n);
        BIND_EBO(indices, m);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (8 * sizeof(float)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*) (11 * sizeof(float)));

        UNBIND_OBJECT;
        return {vao, vbo, ebo};
    }

    void cache_append(std::map<std::string, std::tuple<unsigned int, unsigned int>>& cache, unsigned int a, unsigned int b)
    {
        std::string uid = (a <= b)
                ? (std::to_string(a) + "-" + std::to_string(b))
                : (std::to_string(b) + "-" + std::to_string(a))
                ;
        if (cache.find(uid) == cache.end())
            cache[uid] = std::move(std::tuple<unsigned int, unsigned int>(a, b));
    }

    std::vector<unsigned int> parse2l(const std::vector<unsigned int>& indices)
    {
        std::map<std::string, std::tuple<unsigned int, unsigned int>> cache;
        unsigned int index = 0;
        while (index + 3 < indices.size()) {
            cache_append(cache, indices[index    ], indices[index + 1]);
            cache_append(cache, indices[index    ], indices[index + 2]);
            cache_append(cache, indices[index + 1], indices[index + 2]);
            index = index + 3;
        }
        std::vector<unsigned int> lines;
        for (auto& kv : cache) {
            lines.emplace_back(std::get<0>(kv.second));
            lines.emplace_back(std::get<1>(kv.second));
        }
        return lines;
    }

    std::vector<unsigned int> parse2p(const std::vector<unsigned int>& indices)
    {
        std::set<unsigned int> cache;
        unsigned int index = 0;
        while (index + 3 < indices.size()) {
            cache.insert(indices[index]);
            cache.insert(indices[index + 1]);
            cache.insert(indices[index + 2]);
            index = index + 3;
        }
        std::vector<unsigned int> points;
        for (auto& p : cache) {
            points.emplace_back(p);
        }
        return points;
    }

    material AI::decompose1m(const aiScene* scene, const aiMesh* ai_mesh)
    {
        aiMaterial* material_ai = scene->mMaterials[ai_mesh->mMaterialIndex];
        material tmp_m; aiColor3D color;
        glm::vec3 emissive_k, transparent_k, reflective_k;
        float reflectivity_k = 0.f, opacity_k = 0.f, shininess_k = 0.f, shininess_strength_k = 0.f, refracti_k = 0.f;
        int wireframe_k = 0, twosided_k = 0, shading_model_k = 0, blend_func_k = 0;
        material_ai->Get(AI_MATKEY_COLOR_AMBIENT, color);
        tmp_m.Ka = glm::vec3(color.r, color.g, color.b);
        material_ai->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        tmp_m.Kd = glm::vec3(color.r, color.g, color.b);
        material_ai->Get(AI_MATKEY_COLOR_SPECULAR, color);
        tmp_m.Ks = glm::vec3(color.r, color.g, color.b);
        material_ai->Get(AI_MATKEY_COLOR_EMISSIVE, color);
        emissive_k    = glm::vec3(color.r, color.g, color.b);
        material_ai->Get(AI_MATKEY_COLOR_TRANSPARENT, color);
        transparent_k = glm::vec3(color.r, color.g, color.b);
        material_ai->Get(AI_MATKEY_COLOR_REFLECTIVE, color);
        reflective_k  = glm::vec3(color.r, color.g, color.b);
        material_ai->Get(AI_MATKEY_REFLECTIVITY, reflectivity_k);
        material_ai->Get(AI_MATKEY_OPACITY, opacity_k);
        material_ai->Get(AI_MATKEY_SHININESS, shininess_k);
        material_ai->Get(AI_MATKEY_SHININESS_STRENGTH, shininess_strength_k);
        material_ai->Get(AI_MATKEY_REFRACTI, refracti_k);
        material_ai->Get(AI_MATKEY_ENABLE_WIREFRAME, wireframe_k);
        material_ai->Get(AI_MATKEY_TWOSIDED, twosided_k);
        material_ai->Get(AI_MATKEY_SHADING_MODEL, shading_model_k);
        material_ai->Get(AI_MATKEY_BLEND_FUNC, blend_func_k);

        std::cout << "Material (" << material_ai->GetName().C_Str() << "):"  << std::endl
        << "    ambient(Ka)        : " << tmp_m.Ka.r << ", " << tmp_m.Ka.g << ", " << tmp_m.Ka.b << std::endl
        << "    diffuse(Kd)        : " << tmp_m.Kd.r << ", " << tmp_m.Kd.g << ", " << tmp_m.Kd.b << std::endl
        << "    specular(Ks)       : " << tmp_m.Ks.r << ", " << tmp_m.Ks.g << ", " << tmp_m.Ks.b << std::endl
        << "    map_Ka(number)     : " << aiGetMaterialTextureCount(material_ai, aiTextureType_AMBIENT)  << std::endl
        << "    map_Kd(number)     : " << aiGetMaterialTextureCount(material_ai, aiTextureType_DIFFUSE)  << std::endl
        << "    map_Ks(number)     : " << aiGetMaterialTextureCount(material_ai, aiTextureType_SPECULAR) << std::endl
        << "    emissive           : " << emissive_k.r    << ", " << emissive_k.g    << ", " << emissive_k.b    << std::endl
        << "    transparent        : " << transparent_k.r << ", " << transparent_k.g << ", " << transparent_k.b << std::endl
        << "    reflective         : " << reflective_k.r  << ", " << reflective_k.g  << ", " << reflective_k.b  << std::endl
        << "    reflectivity       : " << reflectivity_k          << std::endl
        << "    opacity            : " << opacity_k               << std::endl
        << "    shininess          : " << shininess_k             << std::endl
        << "    shininess strength : " << shininess_strength_k    << std::endl
        << "    refracti           : " << refracti_k              << std::endl
        << "    wireframe(bool)    : " << wireframe_k             << std::endl
        << "    twosided(bool)     : " << twosided_k              << std::endl
        << "    shading model      : " << shading_model_k         << std::endl
        << "    blend func(bool)   : " << blend_func_k            << std::endl
        ;
        tmp_m.shininess = shininess_k;
        if (aiGetMaterialTextureCount(material_ai, aiTextureType_DIFFUSE)) {
            aiString tex_p;
            material_ai->GetTexture(aiTextureType_DIFFUSE, 0, &tex_p);
            tmp_m.Td.path = std::string(tex_p.C_Str());
        }
        if (material_ai->GetTextureCount(aiTextureType_SPECULAR)) {
            aiString tex_p;
            material_ai->GetTexture(aiTextureType_SPECULAR, 0, &tex_p);
            tmp_m.Ts.path = std::string(tex_p.C_Str());
        }
        if (material_ai->GetTextureCount(aiTextureType_AMBIENT)) {
            aiString tex_p;
            material_ai->GetTexture(aiTextureType_AMBIENT, 0, &tex_p);
            tmp_m.Ta.path = std::string(tex_p.C_Str());
        }
        return tmp_m;
    }

    mesh<is_1p> AI::decompose1p(const aiScene *scene, const aiMesh *ai_mesh)
    {
        mesh<is_1p> mesh;
        for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++) {
            vertex1p v = {};
            v.pos = glm::vec3(ai_mesh->mVertices[i].x, ai_mesh->mVertices[i].y, ai_mesh->mVertices[i].z);
            mesh.edge_max.x = v.pos.x > mesh.edge_max.x ? v.pos.x : mesh.edge_max.x;
            mesh.edge_min.x = v.pos.x < mesh.edge_min.x ? v.pos.x : mesh.edge_min.x;
            mesh.edge_max.y = v.pos.y > mesh.edge_max.y ? v.pos.y : mesh.edge_max.y;
            mesh.edge_min.y = v.pos.y < mesh.edge_min.y ? v.pos.y : mesh.edge_min.y;
            mesh.edge_max.z = v.pos.z > mesh.edge_max.z ? v.pos.z : mesh.edge_max.z;
            mesh.edge_min.z = v.pos.z < mesh.edge_min.z ? v.pos.z : mesh.edge_min.z;
            mesh.vertices.emplace_back(v);
        }
        for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++) {
            const aiFace face = ai_mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                mesh.triangles.emplace_back(face.mIndices[j]);
            }
        }
        return mesh;
    }

    mesh<is_2c> AI::decompose2c(const aiScene* scene, const aiMesh* ai_mesh)
    {
        mesh<is_2c> mesh;
        for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++) {
            vertex2c v = {};
            v.pos   = glm::vec3(ai_mesh->mVertices[i].x, ai_mesh->mVertices[i].y, ai_mesh->mVertices[i].z);
            v.color = glm::vec3(0);
            mesh.edge_max.x = v.pos.x > mesh.edge_max.x ? v.pos.x : mesh.edge_max.x;
            mesh.edge_min.x = v.pos.x < mesh.edge_min.x ? v.pos.x : mesh.edge_min.x;
            mesh.edge_max.y = v.pos.y > mesh.edge_max.y ? v.pos.y : mesh.edge_max.y;
            mesh.edge_min.y = v.pos.y < mesh.edge_min.y ? v.pos.y : mesh.edge_min.y;
            mesh.edge_max.z = v.pos.z > mesh.edge_max.z ? v.pos.z : mesh.edge_max.z;
            mesh.edge_min.z = v.pos.z < mesh.edge_min.z ? v.pos.z : mesh.edge_min.z;
            mesh.vertices.emplace_back(v);
        }
        for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++) {
            const aiFace face = ai_mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                mesh.triangles.emplace_back(face.mIndices[j]);
            }
        }
        return mesh;
    }

    mesh<is_2n> AI::decompose2n(const aiScene* scene, const aiMesh* ai_mesh)
    {
        mesh<is_2n> mesh;
        for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++) {
            vertex2n v = {};
            v.pos  = glm::vec3(ai_mesh->mVertices[i].x, ai_mesh->mVertices[i].y, ai_mesh->mVertices[i].z);
            v.norm = glm::vec3(ai_mesh->mNormals[i].x, ai_mesh->mNormals[i].y, ai_mesh->mNormals[i].z);
            mesh.edge_max.x = v.pos.x > mesh.edge_max.x ? v.pos.x : mesh.edge_max.x;
            mesh.edge_min.x = v.pos.x < mesh.edge_min.x ? v.pos.x : mesh.edge_min.x;
            mesh.edge_max.y = v.pos.y > mesh.edge_max.y ? v.pos.y : mesh.edge_max.y;
            mesh.edge_min.y = v.pos.y < mesh.edge_min.y ? v.pos.y : mesh.edge_min.y;
            mesh.edge_max.z = v.pos.z > mesh.edge_max.z ? v.pos.z : mesh.edge_max.z;
            mesh.edge_min.z = v.pos.z < mesh.edge_min.z ? v.pos.z : mesh.edge_min.z;
            mesh.vertices.emplace_back(v);
        }
        for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++) {
            const aiFace face = ai_mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                mesh.triangles.emplace_back(face.mIndices[j]);
            }
        }
        return mesh;
    }

    mesh<is_3c> AI::decompose3c(const aiScene* scene, const aiMesh* ai_mesh)
    {
        mesh<is_3c> mesh;
        for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++) {
            vertex3c v = {};
            v.pos   = glm::vec3(ai_mesh->mVertices[i].x, ai_mesh->mVertices[i].y, ai_mesh->mVertices[i].z);
            v.norm  = glm::vec3(ai_mesh->mNormals[i].x, ai_mesh->mNormals[i].y, ai_mesh->mNormals[i].z);
            v.color = glm::vec3(0);
            mesh.edge_max.x = v.pos.x > mesh.edge_max.x ? v.pos.x : mesh.edge_max.x;
            mesh.edge_min.x = v.pos.x < mesh.edge_min.x ? v.pos.x : mesh.edge_min.x;
            mesh.edge_max.y = v.pos.y > mesh.edge_max.y ? v.pos.y : mesh.edge_max.y;
            mesh.edge_min.y = v.pos.y < mesh.edge_min.y ? v.pos.y : mesh.edge_min.y;
            mesh.edge_max.z = v.pos.z > mesh.edge_max.z ? v.pos.z : mesh.edge_max.z;
            mesh.edge_min.z = v.pos.z < mesh.edge_min.z ? v.pos.z : mesh.edge_min.z;
            mesh.vertices.emplace_back(v);
        }
        for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++) {
            const aiFace face = ai_mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                mesh.triangles.emplace_back(face.mIndices[j]);
            }
        }
        return mesh;
    }

    mesh<is_3t> AI::decompose3t(const aiScene* scene, const aiMesh* ai_mesh)
    {
        mesh<is_3t> mesh;
        for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++) {
            vertex3t v = {};
            v.pos   = glm::vec3(ai_mesh->mVertices[i].x, ai_mesh->mVertices[i].y, ai_mesh->mVertices[i].z);
            v.norm  = glm::vec3(ai_mesh->mNormals[i].x, ai_mesh->mNormals[i].y, ai_mesh->mNormals[i].z);
            v.texCo = ai_mesh->mTextureCoords[0] ? glm::vec2(ai_mesh->mTextureCoords[0][i].x, ai_mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f);
            mesh.edge_max.x = v.pos.x > mesh.edge_max.x ? v.pos.x : mesh.edge_max.x;
            mesh.edge_min.x = v.pos.x < mesh.edge_min.x ? v.pos.x : mesh.edge_min.x;
            mesh.edge_max.y = v.pos.y > mesh.edge_max.y ? v.pos.y : mesh.edge_max.y;
            mesh.edge_min.y = v.pos.y < mesh.edge_min.y ? v.pos.y : mesh.edge_min.y;
            mesh.edge_max.z = v.pos.z > mesh.edge_max.z ? v.pos.z : mesh.edge_max.z;
            mesh.edge_min.z = v.pos.z < mesh.edge_min.z ? v.pos.z : mesh.edge_min.z;
            mesh.vertices.emplace_back(v);
        }
        for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++) {
            const aiFace face = ai_mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                mesh.triangles.emplace_back(face.mIndices[j]);
            }
        }
        return mesh;
    }

    mesh<is_5b> AI::decompose5b(const aiScene* scene, const aiMesh* ai_mesh)
    {
        mesh<is_5b> mesh;
        for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++) {
            vertex5b v = {};
            v.pos   = glm::vec3(ai_mesh->mVertices[i].x, ai_mesh->mVertices[i].y, ai_mesh->mVertices[i].z);
            v.norm  = glm::vec3(ai_mesh->mNormals[i].x, ai_mesh->mNormals[i].y, ai_mesh->mNormals[i].z);
            v.texCo = ai_mesh->mTextureCoords[0] ? glm::vec2(ai_mesh->mTextureCoords[0][i].x, ai_mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f);
            v.tan   = glm::vec3(ai_mesh->mTangents[i].x, ai_mesh->mTangents[i].y, ai_mesh->mTangents[i].z);
            v.bitan = glm::vec3(ai_mesh->mBitangents[i].x, ai_mesh->mBitangents[i].y, ai_mesh->mBitangents[i].z);
            mesh.edge_max.x = v.pos.x > mesh.edge_max.x ? v.pos.x : mesh.edge_max.x;
            mesh.edge_min.x = v.pos.x < mesh.edge_min.x ? v.pos.x : mesh.edge_min.x;
            mesh.edge_max.y = v.pos.y > mesh.edge_max.y ? v.pos.y : mesh.edge_max.y;
            mesh.edge_min.y = v.pos.y < mesh.edge_min.y ? v.pos.y : mesh.edge_min.y;
            mesh.edge_max.z = v.pos.z > mesh.edge_max.z ? v.pos.z : mesh.edge_max.z;
            mesh.edge_min.z = v.pos.z < mesh.edge_min.z ? v.pos.z : mesh.edge_min.z;
            mesh.vertices.emplace_back(v);
        }
        for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++) {
            const aiFace face = ai_mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                mesh.triangles.emplace_back(face.mIndices[j]);
            }
        }
        return mesh;
    }

    void AI::iterate1p(const aiScene* scene, const aiNode* node, std::vector<mesh1p>& meshes, bool m)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            mesh1p mesh = std::move(decompose1p(scene, scene->mMeshes[node->mMeshes[i]]));
            if (m) mesh.ma = decompose1m(scene, scene->mMeshes[node->mMeshes[i]]);
            meshes.emplace_back(mesh);
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
            iterate1p(scene, node->mChildren[i], meshes, m);
    }

    void AI::iterate2c(const aiScene* scene, const aiNode* node, std::vector<mesh2c>& meshes, bool m)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            mesh2c mesh = std::move(decompose2c(scene, scene->mMeshes[node->mMeshes[i]]));
            if (m) mesh.ma = decompose1m(scene, scene->mMeshes[node->mMeshes[i]]);
            meshes.emplace_back(mesh);
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
            iterate2c(scene, node->mChildren[i], meshes, m);
    }

    void AI::iterate2n(const aiScene* scene, const aiNode* node, std::vector<mesh2n>& meshes, bool m)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            mesh2n mesh = std::move(decompose2n(scene, scene->mMeshes[node->mMeshes[i]]));
            if (m) mesh.ma = decompose1m(scene, scene->mMeshes[node->mMeshes[i]]);
            meshes.emplace_back(mesh);
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
            iterate2n(scene, node->mChildren[i], meshes, m);
    }

    void AI::iterate3c(const aiScene* scene, const aiNode* node, std::vector<mesh3c>& meshes, bool m)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            mesh3c mesh = std::move(decompose3c(scene, scene->mMeshes[node->mMeshes[i]]));
            if (m) mesh.ma = decompose1m(scene, scene->mMeshes[node->mMeshes[i]]);
            meshes.emplace_back(mesh);
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
            iterate3c(scene, node->mChildren[i], meshes, m);
    }

    void AI::iterate3t(const aiScene* scene, const aiNode* node, std::vector<mesh3t>& meshes, bool m)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            mesh3t mesh = std::move(decompose3t(scene, scene->mMeshes[node->mMeshes[i]]));
            if (m) mesh.ma = decompose1m(scene, scene->mMeshes[node->mMeshes[i]]);
            meshes.emplace_back(mesh);
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
            iterate3t(scene, node->mChildren[i], meshes, m);
    }

    void AI::iterate5b(const aiScene* scene, const aiNode* node, std::vector<mesh5b>& meshes, bool m)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            mesh5b mesh = std::move(decompose5b(scene, scene->mMeshes[node->mMeshes[i]]));
            if (m) mesh.ma = decompose1m(scene, scene->mMeshes[node->mMeshes[i]]);
            meshes.emplace_back(mesh);
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
            iterate5b(scene, node->mChildren[i], meshes, m);
    }

    void three1p::prepare() {
        for (auto& mesh : meshes) {
            mesh.points  = std::move(parse2p(mesh.triangles));
            mesh.lines   = std::move(parse2l(mesh.triangles));
            object3u obj2t = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.triangles[0], mesh.triangles.size());
            object3u obj2p = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.points[0], mesh.points.size());
            object3u obj2l = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.lines[0], mesh.lines.size());
            objects.emplace_back(std::tuple(obj2t, obj2l, obj2p));
        }
    }
    void three1p::render() {
        for (int i = 0; i < meshes.size(); i++) {
            glBindVertexArray(std::get<0>(objects[i]).vao);
            glDrawElements(GL_TRIANGLES, meshes[i].triangles.size(), GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }
    }

    void three2c::prepare() {
        for (auto& mesh : meshes) {
            mesh.points  = std::move(parse2p(mesh.triangles));
            mesh.lines   = std::move(parse2l(mesh.triangles));
            object3u obj2t = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.triangles[0], mesh.triangles.size());
            object3u obj2p = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.points[0], mesh.points.size());
            object3u obj2l = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.lines[0], mesh.lines.size());
            objects.emplace_back(std::tuple(obj2t, obj2l, obj2p));
        }
    }
    void three2c::render() {
        for (int i = 0; i < meshes.size(); i++) {
            glBindVertexArray(std::get<0>(objects[i]).vao);
            glDrawElements(GL_TRIANGLES, meshes[i].triangles.size(), GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }
    }

    void three2n::prepare() {
        for (auto& mesh : meshes) {
            mesh.points  = std::move(parse2p(mesh.triangles));
            mesh.lines   = std::move(parse2l(mesh.triangles));
            object3u obj2t = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.triangles[0], mesh.triangles.size());
            object3u obj2p = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.points[0], mesh.points.size());
            object3u obj2l = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.lines[0], mesh.lines.size());
            objects.emplace_back(std::tuple(obj2t, obj2l, obj2p));
        }
    }
    void three2n::render() {
        for (int i = 0; i < meshes.size(); i++) {
            glBindVertexArray(std::get<0>(objects[i]).vao);
            glDrawElements(GL_TRIANGLES, meshes[i].triangles.size(), GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }
    }

    void three3c::prepare() {
        for (auto& mesh : meshes) {
            mesh.points  = std::move(parse2p(mesh.triangles));
            mesh.lines   = std::move(parse2l(mesh.triangles));
            object3u obj2t = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.triangles[0], mesh.triangles.size());
            object3u obj2p = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.points[0], mesh.points.size());
            object3u obj2l = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.lines[0], mesh.lines.size());
            objects.emplace_back(std::tuple(obj2t, obj2l, obj2p));
        }
    }
    void three3c::render() {
        for (int i = 0; i < meshes.size(); i++) {
            glBindVertexArray(std::get<0>(objects[i]).vao);
            glDrawElements(GL_TRIANGLES, meshes[i].triangles.size(), GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }
    }

    void three3t::prepare() {
        for (auto& mesh : meshes) {
            mesh.points  = std::move(parse2p(mesh.triangles));
            mesh.lines   = std::move(parse2l(mesh.triangles));
            object3u obj2t = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.triangles[0], mesh.triangles.size());
            object3u obj2p = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.points[0], mesh.points.size());
            object3u obj2l = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.lines[0], mesh.lines.size());
            objects.emplace_back(std::tuple(obj2t, obj2l, obj2p));
        }
    }
    void three3t::render() {
        for (int i = 0; i < meshes.size(); i++) {
            glBindVertexArray(std::get<0>(objects[i]).vao);
            glDrawElements(GL_TRIANGLES, meshes[i].triangles.size(), GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }
    }

    void three5b::prepare() {
        for (auto& mesh : meshes) {
            mesh.points  = std::move(parse2p(mesh.triangles));
            mesh.lines   = std::move(parse2l(mesh.triangles));
            object3u obj2t = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.triangles[0], mesh.triangles.size());
            object3u obj2p = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.points[0], mesh.points.size());
            object3u obj2l = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.lines[0], mesh.lines.size());
            objects.emplace_back(std::tuple(obj2t, obj2l, obj2p));
        }
    }
    void three5b::render() {
        for (int i = 0; i < meshes.size(); i++) {
            glBindVertexArray(std::get<0>(objects[i]).vao);
            glDrawElements(GL_TRIANGLES, meshes[i].triangles.size(), GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }
    }

#define LOAD_3D(q)          \
    three##q three;             \
    Assimp::Importer importer;  \
    const aiScene* scene = importer.ReadFile(  \
            path,                              \
            aiProcess_CalcTangentSpace         \
            | aiProcess_Triangulate            \
            | aiProcess_JoinIdenticalVertices  \
            | aiProcess_SortByPType            \
            );                 \
    if (!scene) {  \
        std::cout << "3D import error: " << importer.GetErrorString() << std::endl;  \
        exit(2);   \
    }              \
    three.meshes.clear();      \
    three.objects.clear();     \
    AI::iterate##q(scene, scene->mRootNode, three.meshes, material_open);  \
    return three

    three1p three3d::load1p(const std::string& path, bool material_open) {LOAD_3D(1p);}
    three2c three3d::load2c(const std::string& path, bool material_open) {LOAD_3D(2c);}
    three2n three3d::load2n(const std::string& path, bool material_open) {LOAD_3D(2n);}
    three3c three3d::load3c(const std::string& path, bool material_open) {LOAD_3D(3c);}
    three3t three3d::load3t(const std::string& path, bool material_open) {LOAD_3D(3t);}
    three5b three3d::load5b(const std::string& path, bool material_open) {LOAD_3D(5b);}
}