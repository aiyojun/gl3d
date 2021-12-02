#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "basic_type.h"
#include "shader.h"

class j_model3d {
public:
    std::string rootPath;
    struct ObjectID {
        unsigned int vao;
        unsigned int vbo;
        unsigned int ebo;
    };
    glm::mat4 transform;
    glm::vec3 pos;
    glm::vec3 rotate;

    std::vector<Mesh> meshes;
    std::vector<ObjectID> objects;


    void load(const char* path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(
                path,
                aiProcess_CalcTangentSpace       |
                aiProcess_Triangulate            |
                aiProcess_JoinIdenticalVertices  |
                aiProcess_SortByPType
                );
        if (!scene) {
            std::cout << importer.GetErrorString() << std::endl;
        } else {
            std::cout << "has meshes : " << (scene->HasMeshes() ? "true\n" : "false\n");
        }
        const aiMaterial* material = scene->mMaterials[0];
        meshes.clear();
        objects.clear();
        decompose_every_mesh(scene, scene->mRootNode, 0);
    }

    void init(const std::string& imageDirectory) {
        rootPath = imageDirectory;
        for (int i = 0; i < meshes.size(); i++) {
            ObjectID obj;
            glGenVertexArrays(1, &obj.vao);
            glGenBuffers(1, &obj.vbo);
            glGenBuffers(1, &obj.ebo);

            glBindVertexArray(obj.vao);
            glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
            glBufferData(GL_ARRAY_BUFFER, meshes[i].vertices.size() * sizeof(Vertex), &(meshes[i].vertices[0]), GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.ebo);
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

            objects.emplace_back(obj);
        }
    }

//            unsigned int diffuseNr = 1;
//            unsigned int specularNr = 1;
//            unsigned int normalNr = 1;
//            unsigned int heightNr = 1;
//            for (int j = 0; j < meshes[i].textures.size(); j++) {
//                glActiveTexture(GL_TEXTURE0 + j);
//                std::string number, name = meshes[i].textures[j].name;
//                if (name == "texture_diffuse") {
//                    number = std::to_string(diffuseNr++);
//                } else if (name == "texture_specular") {
//                    number = std::to_string(specularNr++);
//                } else if (name == "texture_normal") {
//                    number = std::to_string(normalNr++);
//                } else if (name == "texture_height") {
//                    number = std::to_string(heightNr++);
//                }
//                glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), j);
//                glBindTexture(GL_TEXTURE_2D, meshes[i].textures[j].id);
//            }

    void draw(const Shader& shader) {
        for (int i = 0; i < meshes.size(); i++) {
            if (meshes[i].textures.size() == 2) {
                glActiveTexture(GL_TEXTURE0);
                glUniform1i(glGetUniformLocation(shader.ID, "material.diffuse"), 0);
                glBindTexture(GL_TEXTURE_2D, meshes[i].textures[0].id);
                glActiveTexture(GL_TEXTURE1);
                glUniform1i(glGetUniformLocation(shader.ID, "material.specular"), 1);
                glBindTexture(GL_TEXTURE_2D, meshes[i].textures[1].id);
            }

            glBindVertexArray(objects[i].vao);
            glDrawElements(GL_TRIANGLES, meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            if (meshes[i].textures.size() == 2) {
                glActiveTexture(GL_TEXTURE0);
            }
        }
    }

private:
    void get_mesh(const aiMesh* mesh, const aiScene* scene) {
//        std::cout << "Get mesh ; face n : " << mesh->mNumFaces << " ; Has color : " << mesh->HasVertexColors() << std::endl;
        std::cout << "Get mesh ; face n : " << mesh->mNumFaces << std::endl;
        std::vector<Vertex> tmpV;
        std::vector<unsigned int> tmpI;
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            // std::cout
            // << mesh->mVertices[i].x << ", " << mesh->mVertices[i].y << ", " << mesh->mVertices[i].z << "\t; "
            // << mesh->mNormals[i].x << ", " << mesh->mNormals[i].y << ", " << mesh->mNormals[i].z
            // << std::endl;
            Vertex vertex;
            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
            if (mesh->mTextureCoords[0]) {
                vertex.texCoords.s = mesh->mTextureCoords[0][i].x;
                vertex.texCoords.t = mesh->mTextureCoords[0][i].y;
            } else {
                vertex.texCoords = glm::vec2(0.0f, 0.0f);
            }
            vertex.tangent.x = mesh->mTangents[i].x;
            vertex.tangent.y = mesh->mTangents[i].y;
            vertex.tangent.z = mesh->mTangents[i].z;
            vertex.bitangent.x = mesh->mBitangents[i].x;
            vertex.bitangent.y = mesh->mBitangents[i].y;
            vertex.bitangent.z = mesh->mBitangents[i].z;
            tmpV.emplace_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            const aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                tmpI.emplace_back(face.mIndices[j]);
            }
        }

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        Material material1; aiColor3D color;
        material->Get(AI_MATKEY_COLOR_AMBIENT, color);
        material1.Ka = glm::vec3(color.r, color.g, color.b);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        material1.Kd = glm::vec3(color.r, color.g, color.b);
        material->Get(AI_MATKEY_COLOR_SPECULAR, color);
        material->Get(AI_MATKEY_COLOR_SPECULAR, color);
        material->Get(AI_MATKEY_SHININESS, material1.shininess);
        material1.Ks = glm::vec3(color.r, color.g, color.b);

        std::vector<Texture> textures0;
        Texture diffuse_t, specular_t;
        if (getDiffuseTexture(material, diffuse_t) && getSpecularTexture(material, specular_t)) {
            textures0.emplace_back(diffuse_t);
            textures0.emplace_back(specular_t);
        }

//        std::vector<Texture> diffuse_tex = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
//        textures0.insert(textures0.end(), diffuse_tex.begin(), diffuse_tex.end());
//        std::vector<Texture> specular_tex = loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
//        textures0.insert(textures0.end(), diffuse_tex.begin(), diffuse_tex.end());
//        std::vector<Texture> normal_tex = loadMaterialTexture(material, aiTextureType_HEIGHT, "texture_normal");
//        textures0.insert(textures0.end(), diffuse_tex.begin(), diffuse_tex.end());
//        std::vector<Texture> height_tex = loadMaterialTexture(material, aiTextureType_AMBIENT, "texture_height");
//        textures0.insert(textures0.end(), diffuse_tex.begin(), diffuse_tex.end());

        meshes.emplace_back(Mesh(tmpV, tmpI, textures0, material1));
    }

    bool getDiffuseTexture(aiMaterial* material, Texture& tex) {
        if (material->GetTextureCount(aiTextureType_DIFFUSE) == 0) {
            std::cout << "No diffuse texture in material!" << std::endl;
            return false;
        }
        aiString tex_name;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &tex_name);
        Texture te = loadTexture("diffuse", rootPath + "/" + std::string(tex_name.C_Str()));
        tex.id = te.id; tex.name = te.name; tex.path = te.path;
        return true;
    }

    bool getSpecularTexture(aiMaterial* material, Texture& tex) {
        if (material->GetTextureCount(aiTextureType_SPECULAR) == 0) {
            std::cout << "No specular texture in material!" << std::endl;
            return false;
        }
        aiString tex_name;
        material->GetTexture(aiTextureType_SPECULAR, 0, &tex_name);
        Texture te = loadTexture("specular", rootPath + "/" + std::string(tex_name.C_Str()));
        tex.id = te.id; tex.name = te.name; tex.path = te.path;
        return true;
//        return loadTexture("specular", rootPath + "/" + std::string(tex_name.C_Str()));
    }

    std::vector<Texture> loadMaterialTexture(aiMaterial* material, aiTextureType type, const std::string& name) {
        std::vector<Texture> r_textures;
        for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
            aiString s;
            material->GetTexture(type, i, &s);
            bool hasLoaded = false;
            for (const auto& texture : textures) {
                if (texture.first == std::string(s.C_Str())) {
                    r_textures.emplace_back(texture.second);
                    hasLoaded = true;
                    break;
                }
            }
            if (!hasLoaded) {
                Texture tex = loadTexture(name, rootPath + "/" + std::string(s.C_Str()));
                textures[tex.name] = tex;
                r_textures.emplace_back(tex);
            }
        }
        return std::move(r_textures);
    }


    std::map<std::string, Texture> textures;
    Texture loadTexture(const std::string& name, const std::string& path) {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        Texture tex;
        tex.path = path;
        tex.name = name;
        glGenTextures(1, &tex.id);
        GLenum format;
        switch (nrChannels) {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
            default:
                std::cout << "unknown image deep" << std::endl;
                exit(1);
        }
        glBindTexture(GL_TEXTURE_2D, tex.id);
        glTexImage2D(GL_TEXTURE_2D, 0, (GLint) format, (int) width, (int) height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
        return std::move(tex);
    }

    void decompose_every_mesh(const aiScene* scene, const aiNode* node, int depth) {
        std::cout << "Decompose depth : " << depth
        << "; mesh n : " << node->mNumMeshes
        << "; children n : " << node->mNumChildren
        << std::endl;
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            get_mesh(scene->mMeshes[node->mMeshes[i]], scene);
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            decompose_every_mesh(scene, node->mChildren[i], depth + 1);
        }
    }
};