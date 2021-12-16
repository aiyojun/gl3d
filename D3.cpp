#include "D3.h"
#include "stb_image.h"
#include <iostream>

namespace D3 {

#define GEN_OBJECT                                   \
        index_t vao, vbo, ebo;                       \
        glGenVertexArrays(1, &vao);                  \
        glGenBuffers(1, &vbo);                       \
        glGenBuffers(1, &ebo)
#define BIND_VBO(c, p, n)                            \
        glBindVertexArray(vao);                      \
        glBindBuffer(GL_ARRAY_BUFFER, vbo);          \
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) ((n) * sizeof(c)), (p), GL_STATIC_DRAW)
#define BIND_EBO(p, n)                               \
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);  \
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (n) * sizeof(unsigned int), (p), GL_STATIC_DRAW)
#define UNBIND_OBJECT                                \
        glBindVertexArray(0);                        \
        glBindBuffer(GL_ARRAY_BUFFER, 0);            \
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)

    object3u vertex1p::allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m)
    {
        GEN_OBJECT;
        BIND_VBO(vertex1p, vertices, n);
        BIND_EBO(indices, m);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
        UNBIND_OBJECT;
        return {vao, vbo, ebo};
    }
    object3u vertex2c::allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m)
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
    inline object3u vertex2n::allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m)
    {
        return vertex2c::allocate(vertices, n, indices, m);
    }
    object3u vertex3c::allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m)
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
    object3u vertex3t::allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m)
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
    object3u vertex5b::allocate(const void *vertices, unsigned int n, const void *indices, unsigned int m)
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
        std::vector<unsigned int> points(cache.size());
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

    mesh1p AI::decompose1p(const aiScene *scene, const aiMesh *ai_mesh)
    {
        mesh1p mesh;
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

    mesh2c AI::decompose2c(const aiScene* scene, const aiMesh* ai_mesh)
    {
        mesh2c mesh;
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

    mesh2n AI::decompose2n(const aiScene* scene, const aiMesh* ai_mesh)
    {
        mesh2n mesh;
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

    mesh3c AI::decompose3c(const aiScene* scene, const aiMesh* ai_mesh)
    {
        mesh3c mesh;
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

    mesh3t AI::decompose3t(const aiScene* scene, const aiMesh* ai_mesh)
    {
        mesh3t mesh;
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

    mesh5b AI::decompose5b(const aiScene* scene, const aiMesh* ai_mesh)
    {
        mesh5b mesh;
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

#define ITERATE(q)  \
    void AI::iterate##q(const aiScene* scene, const aiNode* node, std::vector<mesh##q>& meshes, bool m)  \
    {  \
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {  \
            mesh##q mesh = std::move(decompose##q(scene, scene->mMeshes[node->mMeshes[i]]));  \
            if (m) mesh.ma = decompose1m(scene, scene->mMeshes[node->mMeshes[i]]);  \
            meshes.emplace_back(mesh);  \
        }  \
        for (unsigned int i = 0; i < node->mNumChildren; i++)  \
            iterate##q(scene, node->mChildren[i], meshes, m);   \
    }

    ITERATE(1p)
    ITERATE(2c)
    ITERATE(2n)
    ITERATE(3c)
    ITERATE(3t)
    ITERATE(5b)

    unsigned int loadTexture(const std::string& path)
    {
        unsigned int r = 0;
        int width, height, nrChannels;
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        glGenTextures(1, &r);
        GLenum format;
        switch (nrChannels) {
            case 1: format = GL_RED; break;
            case 3: format = GL_RGB; break;
            case 4: format = GL_RGBA;break;
            default: std::cout << "unknown image deep" << std::endl; return 0;
        }
        glBindTexture(GL_TEXTURE_2D, r);
        glTexImage2D(GL_TEXTURE_2D, 0, (GLint) format, (int) width, (int) height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
        return r;
    }

    static void _prepare_texture(material& material, bool material_open)
    {
        if (material_open) {
            if (!material.Ta.path.empty()) {
                material.Ta.id = loadTexture(material.Ta.path);
            }
            if (!material.Td.path.empty()) {
                material.Td.id = loadTexture(material.Td.path);
            }
            if (!material.Ts.path.empty()) {
                material.Ts.id = loadTexture(material.Ts.path);
            }
        }
    }

#define SHADER_SET(q) \
    void three##q::shader_set(const shader_func& pre, const shader_func& pos) { \
        ws.pre_ptr = pre; ws.pos_ptr = pos;  \
    }

    SHADER_SET(1p)
    SHADER_SET(2c)
    SHADER_SET(2n)
    SHADER_SET(3c)
    SHADER_SET(3t)
    SHADER_SET(5b)


#define PREPARE(q)  \
    void three##q::prepare() {  \
        for (auto& mesh : meshes) {  \
            mesh.points  = std::move(parse2p(mesh.triangles));  \
            mesh.lines   = std::move(parse2l(mesh.triangles));  \
            object3u obj2t = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.triangles[0], mesh.triangles.size());  \
            object3u obj2p = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.points[0], mesh.points.size());  \
            object3u obj2l = vertex1p::allocate(&mesh.vertices[0], mesh.vertices.size(), &mesh.lines[0], mesh.lines.size());  \
            objects.emplace_back(std::tuple(obj2t, obj2l, obj2p));  \
            _prepare_texture(mesh.ma, material_open);  \
            if (material_open && ws.pre_ptr != nullptr) ws.pre_ptr(&mesh.ma);  \
        }  \
    }

    PREPARE(1p)
    PREPARE(2c)
    PREPARE(2n)
    PREPARE(3c)
    PREPARE(3t)
    PREPARE(5b)

#define RENDER(q)  \
    void three##q::render() {  \
        for (int i = 0; i < meshes.size(); i++) {  \
            glBindVertexArray(std::get<0>(objects[i]).vao);  \
            glDrawElements(GL_TRIANGLES, meshes[i].triangles.size(), GL_UNSIGNED_INT, nullptr);  \
            glBindVertexArray(0);  \
            if (material_open && ws.pos_ptr != nullptr) ws.pos_ptr(&meshes[i].ma);  \
        }  \
    }

    RENDER(1p)
    RENDER(2c)
    RENDER(2n)
    RENDER(3c)
    RENDER(3t)
    RENDER(5b)

#define LOAD3D(q)  \
    three##q load##q(const std::string& path, bool material_open) {  \
        three##q three;             \
        three.ws = {nullptr, nullptr};  \
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
        three.material_open = material_open;       \
        AI::iterate##q(scene, scene->mRootNode, three.meshes, material_open);  \
        return three;  \
    }

    LOAD3D(1p)
    LOAD3D(2c)
    LOAD3D(2n)
    LOAD3D(3c)
    LOAD3D(3t)
    LOAD3D(5b)
}