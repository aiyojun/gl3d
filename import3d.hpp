#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "basic_type.h"

#include <string>
#include <vector>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

class three3d_t {
public:

private:
    bool decompose_material = true;
};

#ifdef IMPORT3D_IMPL
typedef unsigned int indice_t;
Mesh decompose_mesh(
    const aiScene* scene,
    const aiMesh* mesh)
{
    std::vector<Vertex> tmp_v;
    std::vector<indice_t> tmp_i;
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
        tmp_m.Td.path = std::string(tex_p.C_Str()));
    }
    if (material_ai->GetTextureCount(aiTextureType_SPECULAR)) {
        aiString tex_p;
        material_ai->GetTexture(aiTextureType_SPECULAR, 0, &tex_p);
        tmp_m.Ts.path = std::string(tex_p.C_Str()));
    }
    if (material_ai->GetTextureCount(aiTextureType_AMBIENT)) {
        aiString tex_p;
        material_ai->GetTexture(aiTextureType_AMBIENT, 0, &tex_p);
        tmp_m.Ta.path = std::string(tex_p.C_Str()));
    }
    return Mesh(tmp_v, tmp_i, tmp_m);
}
void iterate_mesh(
    const aiScene* scene,
    const aiNode* node, std::vector<Mesh>& meshes, int depth)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
        meshes.emplace_back(decompose_mesh(scene, scene->mMeshes[node->mMeshes[i]]));
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        iterate_mesh(scene, node->mChildren[i], meshes, depth + 1);
}
#endif