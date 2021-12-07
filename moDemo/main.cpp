#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define ALL_IMPL
#include "../shader.hpp"
#include "../D3.hpp"
#include "../smartfs.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

shader_t shader0, shader1;

three3d_t three3D;

void init() {
    smartfs::cache(smartfs::prefix(smartfs::runtime_path(), "gl3d"));
    shader0.init(smartfs::find("color_v.glsl").c_str(),
                 smartfs::find("color_f.glsl").c_str());
    shader1.init(smartfs::find("v.shader").c_str(),
                 smartfs::find("f.shader").c_str());
//    three3D.set_shader(shader0.ID, shader1.ID);
    three3D.load3d(smartfs::find("cube.obj").c_str(), shader0.ID, true);
//    three3D.load3d("../asset/uploads_files_2787791_Mercedes+Benz+GLS+580.obj", false);
    three3D.prepare();
//    shader0.use();
//    shader0.setFloat("material.shininess", 2.0f);
//    shader0.setVec3("light.direction", glm::vec3(-1.0f, -1.0f, -1.0f));
//    shader0.setVec3("light.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
//    shader0.setVec3("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
//    shader0.setVec3("light.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    shader1.use();
    shader1.setFloat("material.shininess", 2.0f);
    shader1.setVec3("light.direction", glm::vec3(-1.0f, -1.0f, -1.0f));
    shader1.setVec3("light.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
    shader1.setVec3("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    shader1.setVec3("light.specular", glm::vec3(0.5f, 0.5f, 0.5f));
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glm::vec3 viewPos = glm::vec3(-2.0f, 2.0f, -2.0f);
    glm::mat4 transform = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), 960.0f / 480, 0.1f, 100.0f);

    shader0.use();
    shader0.setMat4("transform", transform);
    shader0.setMat4("view", view);
    shader0.setMat4("projection", projection);
    shader0.setVec3("viewPos", viewPos);

    shader1.use();
    shader1.setMat4("transform", transform);
    shader1.setMat4("view", view);
    shader1.setMat4("projection", projection);
    shader1.setVec3("viewPos", viewPos);

    three3D.render();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    int width = 960, height = 480;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(500, 400);
    glutCreateWindow("OpenGL Demo");
    glewInit();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    init();

    glutMainLoop();
    return 0;
}