#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_glut.h>
#include <imgui_impl_opengl2.h>
#define ALL_IMPL
#include "../shader.hpp"
#include "../D3.hpp"

float width_w = 960.0f, height_w = 480.0f;

shader_t shader0, shader1;

three3d_t three3D;

glm::vec3 viewPos;
glm::vec3 viewDir;
glm::mat4 transform;
glm::mat4 view;
glm::mat4 projection;

void init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();
    // ImGui::StyleColorsClassic();
    // Setup Platform/Renderer backends
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();

    viewPos = glm::vec3(-2.0f, 2.0f, -2.0f);
    viewDir = glm::vec3(0.0f, 0.0f, 0.0f);
    transform = glm::mat4(1.0f);
    view = glm::lookAt(viewPos, viewDir, glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(60.0f), width_w / height_w, 0.1f, 100.0f);

    shader0.init("../shader/color_v.glsl", "../shader/color_f.glsl");
    shader1.init("../shader/v.shader", "../shader/f.shader");
    three3D.set_shader(shader0.ID, shader1.ID);
    three3D.load3d("../asset/cube.obj", true);
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

    view = glm::lookAt(viewPos, viewDir, glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(60.0f), width_w / height_w, 0.1f, 100.0f);

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

    glUseProgram(0);
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    ImGui::Begin("View Control");
    ImGui::SliderFloat("Eye X", &viewPos.x, -20.0f, 20.0f);
    ImGui::SliderFloat("Eye Y", &viewPos.y, -20.0f, 20.0f);
    ImGui::SliderFloat("Eye Z", &viewPos.z, -20.0f, 20.0f);
    ImGui::SliderFloat("Dir X", &viewDir.x, -20.0f, 20.0f);
    ImGui::SliderFloat("Dir Y", &viewDir.y, -20.0f, 20.0f);
    ImGui::SliderFloat("Dir Z", &viewDir.z, -20.0f, 20.0f);
    ImGui::End();
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    int width = (int) width_w, height = (int) height_w;
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

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    return 0;
}