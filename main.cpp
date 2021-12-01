#include <stdio.h>
#include <string>
#include <iostream>
#include <exception>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "shader.h"
#include "drawing.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "extensions.h"
j_imgui widgets;


Shader shader;

struct MovingOffset {
    double t_x;
    double t_y;
    double t_z;
    double p_x;
    double p_y;
    double p_z;
};
MovingOffset moving;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Mesh(std::vector<Vertex>& v, std::vector<unsigned int>& i) {vertices = v; indices = i;}
};
std::vector<Mesh> meshes;
struct ObjectID {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
};
std::vector<ObjectID> objects;

struct Light {
    ObjectID objectID;
    glm::mat4 model;
    glm::vec3 position;
};
Light lightObject;

void get_mesh(const aiMesh* mesh, const aiScene* scene) {
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
        vertex.color.r = 0.3f;
        vertex.color.g = 0.3f;
        vertex.color.b = 0.3f;
        tmpV.emplace_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            tmpI.emplace_back(face.mIndices[j]);
        }
    }
    meshes.emplace_back(Mesh(tmpV, tmpI));
}

void decompose_every_mesh(const aiScene* scene, const aiNode* node, int depth) {
    std::cout << "Decompose depth : " << depth << "; mesh n : " << node->mNumMeshes << std::endl;
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        get_mesh(scene->mMeshes[node->mMeshes[i]], scene);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        decompose_every_mesh(scene, node->mChildren[i], depth + 1);
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)960/480, 0.1, 100.0);
    // glm::mat4 projection = glm::perspective(
        // glm::radians(60.0f), (float) 960 / (float) 480, 1.0f, 100.0f
        // );
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    gluLookAt(
        // -10, -5, -5.0,
        // 0, 0, 5.0,
        moving.t_x, moving.t_y, moving.t_z,
        moving.p_x, moving.p_y, moving.p_z,
        0.0, 1.0, 0
        );
}

// glm::mat4 lightModel;
// glm::vec3 lightPos(5.0f, 5.0f, 5.0f);

j_line x_scope;
j_line y_scope;
j_line z_scope;

void init() {
    // moving.t_x = -8.0;
    // moving.t_y = 1.0;
    // moving.t_z = -9.0;
    // moving.p_x = 2.0;
    // moving.p_y = 6.0;
    // moving.p_z = 1.0;
    moving.t_x = 0.0;
    moving.t_y = 0.0;
    moving.t_z = -10.0;
    moving.p_x = 0.0;
    moving.p_y = 0.0;
    moving.p_z = 0.0;
    lightObject.position = glm::vec3(0.0f, -10.0f, 0.0f);

//    glClearColor(0, 0, 0, 0);

    x_scope.init(
            glm::vec3(0.0f),
            glm::vec3(3.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            2);
    y_scope.init(
            glm::vec3(0.0f),
            glm::vec3(0.0f, 3.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            2);
    z_scope.init(
            glm::vec3(0.0f),
            glm::vec3(0.0f, 0.0f, 3.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            2);

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
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, color));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        objects.emplace_back(obj);
    }

//    glGenVertexArrays(1, &lightObject.objectID.vao);
//    glGenBuffers(1, &lightObject.objectID.vbo);
//    glBindVertexArray(lightObject.objectID.vao);
//    glBindBuffer(GL_ARRAY_BUFFER, lightObject.objectID.vbo);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &(lightObject.position), GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    lightObject.model = glm::mat4();
//    lightObject.model = glm::translate(lightObject.model, lightObject.position);
//    lightObject.model = glm::scale(lightObject.model, glm::vec3(0.2f));
}



void display() {


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//    // glm::vec3 focus(moving.t_x, moving.t_y, moving.t_z);
    float alpha = atan((moving.p_y - moving.t_y) / (moving.p_z - moving.t_z));
    float beta = atan((moving.p_x - moving.t_x) / (moving.p_z - moving.t_z));
    float gama = 0;
    gluLookAt(
        // -10, -5, -5.0,
        // 0, 0, 5.0,
        moving.t_x, moving.t_y, moving.t_z,
        moving.p_x, moving.p_y, moving.p_z,
        0.0, 1.0, 0.0
        );
//    std::cout << ">> flush" << std::endl;

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    float w = 200.0 * 960.0 * atan(60.0 / 2.0) / 480.0;
    float h = 200.0 * atan(60.0 / 2.0);
    shader.setFloat("width", w);
    shader.setFloat("height", h);
    shader.setFloat("depth", 100.0);
    shader.setFloat("alpha", alpha);
    shader.setFloat("beta", beta);
    shader.setFloat("gama", gama);
    // shader.setVec3("lightPos", 0.0f, -15.0f, 0.0f);
    shader.setVec3("lightPos", lightObject.position);
    shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    shader.setVec3("focus", (float) moving.t_x, (float) moving.t_y, (float) moving.t_z);
//    std::cout << "w: " << w
//    << "; h: " << h
//    << "; alpha: " << alpha
//    << "; beta: " << beta
//    << "; gama: " << gama
//    << std::endl;

    x_scope.draw();
    y_scope.draw();
    z_scope.draw();


//    for (int i = 0; i < meshes.size(); i++) {
//        glBindVertexArray(objects[i].vao);
//        glDrawElements(GL_TRIANGLES, meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
//        glBindVertexArray(0);
//    }

//    glUseProgram(0);
    widgets.beforeDraw();
    widgets.draw();
    widgets.afterDraw();

    glutSwapBuffers();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    double step = 0.2;
    switch (key) {
        case 'A':
        case 'a':
            moving.t_x += -step;
            moving.p_x += -step;
        std::cout<<"-A-" 
        << moving.t_x << "," << moving.t_y <<","<<moving.t_z<<"," 
        << moving.p_x << "," << moving.p_y <<","<<moving.p_z
        << std::endl;
            break;
        case 'S':
        case 's':
        std::cout<<"-S-"<< std::endl;
            moving.t_y += -step;
            moving.p_y += -step;
            break;
        case 'D':
        case 'd':
        std::cout<<"-D-"<< std::endl;
            moving.t_x += step;
            moving.p_x += step;
            break;
        case 'w':
        case 'W':
        std::cout<<"-W-"<< std::endl;
            moving.t_y += step;
            moving.p_y += step;
            break;
        case 'q':
        case 'Q':
        std::cout<<"-Q-"<< std::endl;
            moving.t_z += step;
            moving.p_z += step;
            break;
        case 'e':
        case 'E':
        std::cout<<"-E-"<< std::endl;
            moving.t_z += -step;
            moving.p_z += -step;
            break;
        case 'i':
        case 'I':
            std::cout<<"-up-"<< std::endl;
            moving.p_y += step;
            break;
        case 'k':
        case 'K':
            std::cout<<"-down-"<< std::endl;
            moving.p_y += -step;
            break;
        case 'j':
        case 'J':
            std::cout<<"-left-"<< std::endl;
            moving.p_x += -step;
            break;
        case 'l':
        case 'L':
            std::cout<<"-right-"<< std::endl;
            moving.p_x += step;
            break;
        case 'z':
        case 'Z':
            lightObject.position.x += step;
            break;
        case 'x':
        case 'X':
            lightObject.position.x -= step;
            break;
        case 'c':
        case 'C':
            lightObject.position.z += step;
            break;
        case 'V':
        case 'v':
            lightObject.position.z -= step;
        default:
            std::cout<<"default:" << key << std::endl;
            ;
    }
    glutPostRedisplay();
}


int main(int argc, char** argv) {
    const GLubyte* name = glGetString(GL_VENDOR);            //返回负责当前OpenGL实现厂商的名字
    const GLubyte* biaoshifu = glGetString(GL_RENDERER);    //返回一个渲染器标识符，通常是个硬件平台
    const GLubyte* OpenGLVersion = glGetString(GL_VERSION);    //返回当前OpenGL实现的版本号
    const GLubyte* glsl = glGetString(GL_SHADING_LANGUAGE_VERSION);//返回着色预压编译器版本号
    const GLubyte* gluVersion = gluGetString(GLU_VERSION);    //返回当前GLU工具库版本
    printf("OpenGL实现厂商的名字：%s\n", name);
    printf("渲染器标识符：%s\n", biaoshifu);
    printf("OpenGL实现的版本号：%s\n", OpenGLVersion);
    printf("OpenGL着色语言版本：%s\n", glsl);
    printf("GLU工具库版本：%s\n", gluVersion);

    Assimp::Importer importer;

    std::cout << "vertex : " << offsetof(Vertex, position) << ";" << offsetof(Vertex, normal) << std::endl;
    
    // comments

    std::cout << "importer:" << std::endl;
    try {
        const aiScene* scene = importer.ReadFile(
                "/opt/lea/learn-gl/uploads_files_2787791_Mercedes+Benz+GLS+580.obj",
                aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        std::cout << "mFlags : " << scene->mFlags << std::endl;
        std::cout << "mNumMeshes : " << scene->mNumMeshes << std::endl;
        std::cout << "mNumMaterials : " << scene->mNumMaterials << std::endl;
        const aiMaterial* material = scene->mMaterials[0];
        std::cout << "material : " << material << std::endl;
        if (material) {
            aiString path;
            aiString name;
            material->Get(AI_MATKEY_NAME, name);
            std::cout << "name : " << std::string(name.C_Str()) << std::endl;
            aiColor3D diffuse;
            material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
            std::cout << "diffuse : (" << diffuse.r << ", " << diffuse.g << ", " << diffuse.b << ")" << std::endl;
            aiColor3D ambient;
            material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
            std::cout << "ambient : (" << ambient.r << ", " << ambient.g << ", " << ambient.b << ")" << std::endl;
            aiColor3D specular;
            material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
            std::cout << "specular : (" << specular.r << ", " << specular.g << ", " << specular.b << ")" << std::endl;
            aiColor3D emissive;
            material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
            std::cout << "emissive : (" << emissive.r << ", " << emissive.g << ", " << emissive.b << ")" << std::endl;
            aiColor3D transparent;
            material->Get(AI_MATKEY_COLOR_TRANSPARENT, transparent);
            std::cout << "transparent : (" << transparent.r << ", " << transparent.g << ", " << transparent.b << ")" << std::endl;
            aiColor3D reflective;
            material->Get(AI_MATKEY_COLOR_REFLECTIVE, reflective);
            std::cout << "reflective : (" << reflective.r << ", " << reflective.g << ", " << reflective.b << ")" << std::endl;
            float opacity;
            aiGetMaterialFloat(material, AI_MATKEY_OPACITY, &opacity);
            std::cout << "opacity : " << opacity << std::endl;
            // std::cout << "name : " << std::string(name.C_Str()) << std::endl;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
            std::cout << "path : " << std::string(path.C_Str()) << std::endl;
            std::cout << "mNumProperties : " << material->mNumProperties << std::endl;
            std::cout << "mNumAllocated : " << material->mNumAllocated << std::endl;
        }
        std::cout << "mNumAnimations : " << scene->mNumAnimations << std::endl;
        std::cout << "mNumTextures : " << scene->mNumTextures << std::endl;
        std::cout << "mNumLights : " << scene->mNumLights << std::endl;
        std::cout << "mNumCameras : " << scene->mNumCameras << std::endl;
        if (!scene) {
            std::cout << importer.GetErrorString() << std::endl;
        } else {
            std::cout << "has meshes : " << (scene->HasMeshes() ? "true\n" : "false\n");
        }
        decompose_every_mesh(scene, scene->mRootNode, 0);
        std::cout << ">> decompose over" << std::endl;

        

        // window
        glutInit(&argc, argv);
        #ifdef __FREEGLUT_EXT_H__
        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
//        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        glutInitWindowSize(960, 480);
        glutInitWindowPosition(400, 400);
        glutCreateWindow("glc");
        std::cout << ">> init model" << std::endl;
        glewInit();
        shader.init("./vertex.vs", "./fragment.fs");
        init();
        std::cout << ">> prepare window" << std::endl;
        glutReshapeFunc(&reshape);
        glutDisplayFunc(&display);
        glutKeyboardFunc(&keyboard);

        widgets.init();

        glutMainLoop();
        widgets.close();

        
    } catch (std::exception& e) {
        std::cout << std::string(e.what()) << std::endl;
    }
    return 0;
}