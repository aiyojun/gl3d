#pragma once

#define STB_IMAGE_IMPLEMENTATION
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

typedef unsigned int index_t;

class three3d_t {
public:
    std::vector<Mesh> meshes;
    std::vector<std::tuple<index_t, index_t, index_t>> ids;
    void load3d(const char* path, bool mat = true);
    void prepare(unsigned int shader_id);
    void render(unsigned int shader_id);
private:
    bool decompose_material;
};

#ifdef IMPORT3D_IMPL

Mesh decompose_mesh(
    const aiScene* scene,
    const aiMesh* mesh, bool decompose_material = true)
{
    std::vector<Vertex> tmp_v;
    std::vector<index_t> tmp_i;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position  = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal    = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        vertex.texCoords = mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(-1.0f);
        vertex.tangent   = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
        vertex.bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        tmp_v.emplace_back(vertex);
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            tmp_i.emplace_back(face.mIndices[j]);
    }
    if (!decompose_material)
        return {tmp_v, tmp_i};
    aiMaterial* material_ai = scene->mMaterials[mesh->mMaterialIndex];
    Material tmp_m; aiColor3D color;
    material_ai->Get(AI_MATKEY_COLOR_AMBIENT, color);
    tmp_m.Ka = glm::vec3(color.r, color.g, color.b);
    material_ai->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    tmp_m.Kd = glm::vec3(color.r, color.g, color.b);
    material_ai->Get(AI_MATKEY_COLOR_SPECULAR, color);
    tmp_m.Ks = glm::vec3(color.r, color.g, color.b);
    material_ai->Get(AI_MATKEY_COLOR_SPECULAR, tmp_m.shininess);
    if (material_ai->GetTextureCount(aiTextureType_DIFFUSE)) {
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
    return {tmp_v, tmp_i, tmp_m};
}

void iterate_mesh(
    const aiScene* scene,
    const aiNode* node, std::vector<Mesh>& meshes, bool decompose_material, int depth)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
        meshes.emplace_back(decompose_mesh(scene, scene->mMeshes[node->mMeshes[i]], decompose_material));
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        iterate_mesh(scene, node->mChildren[i], meshes, decompose_material, depth + 1);
}

unsigned int loadTexture(const std::string& path) {
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

void three3d_t::load3d(const char *path, bool mat)
{
    decompose_material = mat;
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path, aiProcess_CalcTangentSpace | aiProcess_Triangulate
        | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
    if (!scene) {
        std::cout << importer.GetErrorString() << std::endl;
    }
    meshes.clear(); ids.clear();
    iterate_mesh(scene, scene->mRootNode, meshes, decompose_material, 0);
}

void three3d_t::prepare(unsigned int shader_id)
{
    for (int i = 0; i < meshes.size(); i++) {
        index_t vao, vbo, ebo;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, meshes[i].vertices.size() * sizeof(Vertex), &(meshes[i].vertices[0]), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes[i].indices.size() * sizeof(unsigned int), &(meshes[i].indices[0]), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texCoords));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        ids.emplace_back(std::tuple<index_t, index_t, index_t>(vao, vbo, ebo));
        if (!decompose_material) return;

        if (!meshes[i].material.Ta.path.empty()) {
            meshes[i].material.Ta.id = loadTexture(meshes[i].material.Ta.path);
            if (meshes[i].material.Ta.id != 0) {
                glUniform1i(glGetUniformLocation(shader_id, "material.ambient"), 0);
            }
        }
        if (!meshes[i].material.Td.path.empty()) {
            meshes[i].material.Td.id = loadTexture(meshes[i].material.Td.path);
            if (meshes[i].material.Td.id != 0) {
                glUniform1i(glGetUniformLocation(shader_id, "material.diffuse"), 0);
            }
        }
        if (!meshes[i].material.Ts.path.empty()) {
            meshes[i].material.Ts.id = loadTexture(meshes[i].material.Ts.path);
            if (meshes[i].material.Ts.id != 0) {
                glUniform1i(glGetUniformLocation(shader_id, "material.specular"), 0);
            }
        }
    }
}

void three3d_t::render(unsigned int /*shader_id*/)
{
    for (int i = 0; i < meshes.size(); i++) {
        if (decompose_material) {
            int j = 0;
            Material& material = meshes[i].material;
            if (!material.Ta.path.empty() && material.Ta.id) {
                glActiveTexture(GL_TEXTURE0 + j);
                glBindTexture(GL_TEXTURE_2D, material.Ta.id);
                j++;
            }
            if (!material.Td.path.empty() && material.Td.id) {
                glActiveTexture(GL_TEXTURE0 + j);
                glBindTexture(GL_TEXTURE_2D, material.Td.id);
                j++;
            }
            if (!material.Ts.path.empty() && material.Ts.id) {
                glActiveTexture(GL_TEXTURE0 + j);
                glBindTexture(GL_TEXTURE_2D, material.Ts.id);
                j++;
            }
        }
        glBindVertexArray(std::get<0>(ids[i]));
        glDrawElements(GL_TRIANGLES, meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        if (decompose_material) glActiveTexture(GL_TEXTURE0);
    }
}
#endif