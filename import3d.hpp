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

};

#ifdef IMPORT3D_IMPL
typedef unsigned int indice_t;
static void decompose_mesh(
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
//    aiMaterial* material_ai = scene->mMaterials[mesh->mMaterialIndex];
//    Material material; aiColor3D color;
//    material_ai->Get(AI_MATKEY_COLOR_AMBIENT, color);
//    material.Ka = glm::vec3(color.r, color.g, color.b);
//    material_ai->Get(AI_MATKEY_COLOR_DIFFUSE, color);
//    material.Kd = glm::vec3(color.r, color.g, color.b);
//    material_ai->Get(AI_MATKEY_COLOR_SPECULAR, color);
//    material.Ks = glm::vec3(color.r, color.g, color.b);
//    material_ai->Get(AI_MATKEY_COLOR_SPECULAR, material.shininess);

}
static void iterate_mesh(
    const aiScene* scene,
    const aiNode* node, int depth)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
        decompose_mesh(scene, scene->mMeshes[node->mMeshes[i]]);
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        iterate_mesh(scene, node->mChildren[i], depth + 1);
}
#endif