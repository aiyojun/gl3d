#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define SHADER_IMPL
#include "../shader.hpp"
#define IMPORT3D_IMPL
#include "../import3d.hpp"

shader_t shader0;
shader_t shader1;

three3d_t three3D;

void init() {
    shader0.init("./color_v.glsl", "./color_f.glsl");
    shader1.init("./v.shader", "./f.shader");
    three3D.set_shader(shader0.ID, shader1.ID);
    three3D.load3d("./cube.obj", true);
//    three3D.load3d("/opt/lea/learn-gl/uploads_files_2787791_Mercedes+Benz+GLS+580.obj", false);
    three3D.prepare();
    std::cout << "---1" << std::endl;

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
    std::cout << "---2" << std::endl;

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
    std::cout << "---3" << std::endl;

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