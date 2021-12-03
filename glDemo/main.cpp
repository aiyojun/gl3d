#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define SHADER_IMPL
#include "../shader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

shader_t shader;

float cube[] = {
        // 1
        0.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        // 2
//        0.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
//        1.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
//        1.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
//        0.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
//        1.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
//        0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,  -1.0f, -1.0f,
        1.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,  -1.0f, -1.0f,
        1.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,  -1.0f, -1.0f,
        0.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,  -1.0f, -1.0f,
        1.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,  -1.0f, -1.0f,
        0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,  -1.0f, -1.0f,
        // 3
        0.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        // 4 up
//        0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  -1.0f, -1.0f,
//        1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  -1.0f, -1.0f,
//        1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  -1.0f, -1.0f,
//        0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  -1.0f, -1.0f,
//        1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  -1.0f, -1.0f,
//        0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  -1.0f, -1.0f,
        0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        // 5 z
        0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        // 6 right
        1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
};
unsigned int cube_indices[] = {
        0, 1, 2, 3, 4, 5,
        6, 7, 8, 9, 10, 11,
        12, 13, 14, 15, 16, 17,
        18, 19, 20,21, 22, 23,
        24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35,
};

unsigned int vao = 0;
unsigned int vbo = 0;
unsigned int ebo = 0;

unsigned int tex_id = 0;

void loadTexture(const std::string& path) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    GLenum format;
    switch (nrChannels) {
        case 1: format = GL_RED; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: std::cout << "unknown image deep" << std::endl; exit(1);
    }
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0, (GLint) format, (int) width, (int) height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

void init() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 6*6*(3+3+2)*sizeof(float), &cube[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*6*sizeof(unsigned int), &cube_indices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    loadTexture("../asset/wall.jpg");

    shader.use();

    glUniform1i(glGetUniformLocation(shader.ID, "material.diffuse"), 0);
    shader.setVec3("material.ambient", glm::vec3(0.0f));
    shader.setVec3("material.Kd", glm::vec3(1.0f, 1.0f, 0.0f));
    shader.setFloat("material.shininess", 2.0f);

    shader.setVec3("light.direction", glm::vec3(-1.0f, -1.0f, -1.0f));
    shader.setVec3("light.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setVec3("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setVec3("light.specular", glm::vec3(0.5f, 0.5f, 0.5f));
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

    shader.setMat4("transform", transform);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setVec3("viewPos", viewPos);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);

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

    shader.init("../shader/v.shader", "../shader/f.shader");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    init();

    glutMainLoop();
    return 0;
}