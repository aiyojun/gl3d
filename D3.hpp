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

class three3d_t {
public:
    glm::vec3 edge_min = glm::vec3(0.f), edge_max = glm::vec3(0.f);
    Material default_m;
    unsigned int shader_uid;
//    bool no_fragment_variable = false;
    int attribute_n = 3;
    std::vector<int> attributes_m = {3, 3, 2};
    std::vector<Mesh> meshes;
    std::vector<std::tuple<index_t, index_t, index_t>> ids;
//    std::vector<unsigned int> shader_ids;
//    void set_shader(unsigned int shader);
//    void set_shader(unsigned int shader0, unsigned int shader1);
//    void set_not_normal();
    void load3d(const std::string& path, unsigned int shader, bool mat = true);
    void prepare();
    void render();
private:
    Mesh decompose_mesh(
            const aiScene* scene,
            const aiMesh* mesh,
            bool decompose_material = true,
            int normal_factor = 1);
    void iterate_mesh(
            const aiScene* scene,
            const aiNode* node,
            std::vector<Mesh>& meshes,
            bool decompose_material,
            int depth, int normal_factor = 1);
    int normal_factor = 1;
    bool decompose_material;
};

#if defined(D3_IMPL) || defined(ALL_IMPL)

//inline void three3d_t::set_not_normal()
//{normal_factor = -1;}

Mesh three3d_t::decompose_mesh(
    const aiScene* scene,
    const aiMesh* mesh, bool decompose_material, int normal_factor)
{
    std::vector<Vertex> tmp_v;
    std::vector<index_t> tmp_i;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position  = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal    = glm::vec3(mesh->mNormals[i].x * (float) normal_factor, mesh->mNormals[i].y * (float) normal_factor, mesh->mNormals[i].z * (float) normal_factor);
        vertex.texCoords = mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f);
        // vertex.tangent   = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
        // vertex.bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        if (vertex.position.x > edge_max.x) edge_max.x = vertex.position.x;
        if (vertex.position.x < edge_min.x) edge_min.x = vertex.position.x;
        if (vertex.position.y > edge_max.y) edge_max.y = vertex.position.y;
        if (vertex.position.y < edge_min.y) edge_min.y = vertex.position.y;
        if (vertex.position.z > edge_max.z) edge_max.z = vertex.position.z;
        if (vertex.position.z < edge_min.z) edge_min.z = vertex.position.z;
        tmp_v.emplace_back(vertex);
        // std::cout << "-- vertex : " << mesh->mVertices[i].x << ", " << mesh->mVertices[i].y << ", " << mesh->mVertices[i].z
        //     << "; normal : " << mesh->mNormals[i].x << ", " << mesh->mNormals[i].y << ", " << mesh->mNormals[i].z
        //     << "; tex : " << vertex.texCoords.x << ", " << vertex.texCoords.y
        //     << std::endl;
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            tmp_i.emplace_back(face.mIndices[j]);
        }
    }
    if (!decompose_material)
        return {tmp_v, tmp_i};
    //std::cout << "-- get material pointer, material index : " << mesh->mMaterialIndex << std::endl;
    aiMaterial* material_ai = scene->mMaterials[mesh->mMaterialIndex];
	//const aiMaterial* copy_ptr = material_ai;
	//std::cout << "-- all materials n : " << scene->mNumMaterials << " ; material pointer : " << copy_ptr << std::endl;
	Material tmp_m; aiColor3D color;
	glm::vec3 emissive_k, transparent_k, reflective_k;
	float reflectivity_k = 0.f, opacity_k = 0.f, shininess_k = 0.f, shininess_strength_k = 0.f, refracti_k = 0.f;
	int wireframe_k = 0, twosided_k = 0, shading_model_k = 0, blend_func_k = 0;
    material_ai->Get(AI_MATKEY_COLOR_AMBIENT, color);
    tmp_m.Ka = glm::vec3(color.r, color.g, color.b);
	//std::cout << "-- get material (" << std::string(material_ai->GetName().C_Str()) << ") Ka : " << tmp_m.Ka.r << ", " << tmp_m.Ka.g << ", " << tmp_m.Ka.b << std::endl;
    material_ai->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    tmp_m.Kd = glm::vec3(color.r, color.g, color.b);
	//std::cout << "-- get material (" << std::string(material_ai->GetName().C_Str()) << ") Kd : " << tmp_m.Kd.r << ", " << tmp_m.Kd.g << ", " << tmp_m.Kd.b << std::endl;
    material_ai->Get(AI_MATKEY_COLOR_SPECULAR, color);
    tmp_m.Ks = glm::vec3(color.r, color.g, color.b);
	//std::cout << "-- get material (" << std::string(material_ai->GetName().C_Str()) << ") Ks : " << tmp_m.Ks.r << ", " << tmp_m.Ks.g << ", " << tmp_m.Ks.b << std::endl;
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
	//material_ai->Get(AI_MATKEY_COLOR_SPECULAR, tmp_m.shininess);
	//std::cout << "-- all materials n : " << scene->mNumMaterials << " ; material pointer : " << copy_ptr << std::endl;
    //std::cout << "-- get material diffuse texture count , properties n : " << material_ai->mNumProperties << std::endl;
    if (aiGetMaterialTextureCount(material_ai, aiTextureType_DIFFUSE)) {
        //std::cout << "-- get material diffuse texture count over" << std::endl;
        aiString tex_p;
		//material_ai->GetTextureCount(aiTextureType_DIFFUSE)
		//aiGetMaterialTextureCount(material_ai, aiTextureType_DIFFUSE)
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

void three3d_t::iterate_mesh(
    const aiScene* scene,
    const aiNode* node, 
    std::vector<Mesh>& meshes, 
    bool decompose_material, 
    /* reserved for debugging */ int depth, int normal_factor)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
        meshes.emplace_back(decompose_mesh(scene, scene->mMeshes[node->mMeshes[i]], decompose_material, normal_factor));
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        iterate_mesh(scene, node->mChildren[i], meshes, decompose_material, depth + 1, normal_factor);
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

//void three3d_t::set_shader(unsigned int shader)
//{shader_ids.clear();shader_ids.emplace_back(shader);}
//
//void three3d_t::set_shader(unsigned int shader0, unsigned int shader1)
//{shader_ids.clear();shader_ids.emplace_back(shader0);shader_ids.emplace_back(shader1);}

void three3d_t::load3d(const std::string& path, unsigned int shader, bool mat)
{
    shader_uid = shader;
    decompose_material = mat;
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_CalcTangentSpace
        | aiProcess_Triangulate
        | aiProcess_JoinIdenticalVertices
        | aiProcess_SortByPType
        );
    if (!scene) {
        std::cout << "3D import error: " << importer.GetErrorString() << std::endl;
        exit(2);
    }
    meshes.clear(); ids.clear();
    iterate_mesh(scene, scene->mRootNode, meshes, decompose_material, 0, normal_factor);
}

void three3d_t::prepare()
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

        int offset = 0;
        for (int j = 0; j < attribute_n; j++) {
            glEnableVertexAttribArray(j);
            glVertexAttribPointer(j, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (offset * sizeof(float)));
            offset += attributes_m[j];
        }
        // glEnableVertexAttribArray(0);
        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
        // glEnableVertexAttribArray(1);
        // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
        // glEnableVertexAttribArray(2);
        // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        ids.emplace_back(std::tuple<index_t, index_t, index_t>(vao, vbo, ebo));
        if (!decompose_material) {
//            if (!no_fragment_variable) {
            glUseProgram(shader_uid);
            glUniform1i(glGetUniformLocation(shader_uid, "rendering"), 100);
        //    meshes[i].material.shininess = 0.f;
        //    meshes[i].material.Ka = glm::vec3(1.f, 1.f, 1.f);
        //    meshes[i].material.Kd = glm::vec3(0.5f, 0.5f, 0.5f);
        //    meshes[i].material.Ks = glm::vec3(0.0f, 0.0f, 0.0f);
            glUniform1f(glGetUniformLocation(shader_uid, "material.shininess"), default_m.shininess);
            glUniform3fv(glGetUniformLocation(shader_uid, "material.Ka"), 1, &(default_m.Ka[0]));
            glUniform3fv(glGetUniformLocation(shader_uid, "material.Kd"), 1, &(default_m.Kd[0]));
            glUniform3fv(glGetUniformLocation(shader_uid, "material.Ks"), 1, &(default_m.Ks[0]));
//            }
            continue;
        }

        int j = 100, rendering = 0;
        glUseProgram(shader_uid);
        if (!meshes[i].material.Td.path.empty()) {
            meshes[i].material.Td.id = loadTexture(meshes[i].material.Td.path);
            if (meshes[i].material.Td.id != 0) {
                glUniform1i(glGetUniformLocation(shader_uid, "material.map_Kd"), j++);
                rendering = 200;
            }
        }
        if (!meshes[i].material.Ta.path.empty()) {
            meshes[i].material.Ta.id = loadTexture(meshes[i].material.Ta.path);
            if (meshes[i].material.Ta.id != 0) {
                glUniform1i(glGetUniformLocation(shader_uid, "material.map_Ka"), j++);
                rendering += 001;
            }
        }
        if (!meshes[i].material.Ts.path.empty()) {
            meshes[i].material.Ts.id = loadTexture(meshes[i].material.Ts.path);
            if (meshes[i].material.Ts.id != 0) {
                glUniform1i(glGetUniformLocation(shader_uid, "material.map_Ks"), j++);
                rendering += 010;
            }
        }
        glUseProgram(shader_uid);
        glUniform1i(glGetUniformLocation(shader_uid, "rendering"), rendering);
        //std::cout << ">> xx j : " << j << "; decompose : " << decompose_material << std::endl;
        if (j == 0) {
//            meshes[i].material.shininess = 0.f;
//            meshes[i].material.Ka = glm::vec3(1.f, 1.f, 1.f);
//            meshes[i].material.Kd = glm::vec3(0.5f, 0.5f, 0.5f);
//            meshes[i].material.Ks = glm::vec3(0.0f, 0.0f, 0.0f);
            glUseProgram(shader_uid);
            // if (decompose_material) {
//            glUniform1i(glGetUniformLocation(shader_ids[0], "rendering"), rendering);
            glUniform1f(glGetUniformLocation(shader_uid, "material.shininess"), meshes[i].material.shininess);
            glUniform3fv(glGetUniformLocation(shader_uid, "material.Ka"), 1, &(meshes[i].material.Ka[0]));
            glUniform3fv(glGetUniformLocation(shader_uid, "material.Kd"), 1, &(meshes[i].material.Kd[0]));
            glUniform3fv(glGetUniformLocation(shader_uid, "material.Ks"), 1, &(meshes[i].material.Ks[0]));
            // } else {
            //     meshes[i].material.shininess = 2.f;
            //     meshes[i].material.Ks = glm::vec3(0.5f, 0.5f, 0.5f);
            //     meshes[i].material.Kd = glm::vec3(1.f, 1.f, 1.f);
            //     glUniform1f(glGetUniformLocation(shader_ids[0], "material.shininess"), meshes[i].material.shininess);
            //     glUniform3fv(glGetUniformLocation(shader_ids[0], "material.diffuse"), 1, &(meshes[i].material.Kd[0]));
            //     glUniform3fv(glGetUniformLocation(shader_ids[0], "material.specular"), 1, &(meshes[i].material.Ks[0]));
            // }

        }
    }

//    std::cout << "-- --- --- ---\n";
//    std::cout << "-- mesh size  : " << meshes.size() << std::endl;
//    std::cout << "-- mesh index : " << meshes[0].indices.size() << std::endl;
}

void three3d_t::render()
{
    for (int i = 0; i < meshes.size(); i++) {
        int rendering = 0;
        if (decompose_material) {
            int j = 0;
            Material& material = meshes[i].material;
            if (!material.Ta.path.empty() && material.Ta.id) {
                glActiveTexture(GL_TEXTURE0 + j);
                glBindTexture(GL_TEXTURE_2D, material.Ta.id);
                rendering += 1;
                j++;
            }
            if (!material.Td.path.empty() && material.Td.id) {
                glActiveTexture(GL_TEXTURE0 + j);
                glBindTexture(GL_TEXTURE_2D, material.Td.id);
                rendering += 200;
                j++;
            }
            if (!material.Ts.path.empty() && material.Ts.id) {
                glActiveTexture(GL_TEXTURE0 + j);
                glBindTexture(GL_TEXTURE_2D, material.Ts.id);
                rendering += 10;
                j++;
            }
            /* Here, watch out use-shader position! It must before shader.set(...). */
//            glUseProgram(j > 0 && shader_ids.size() > 1 ? shader_ids[1] : shader_ids[0]);
        }// else {
//            if (no_fragment_variable) {
//                glUseProgram(shader_ids[0]);
//            }
//        }
        glUseProgram(shader_uid);
        glUniform1i(glGetUniformLocation(shader_uid, "rendering"), rendering);
        glBindVertexArray(std::get<0>(ids[i]));
        glDrawElements(GL_TRIANGLES, meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
#endif