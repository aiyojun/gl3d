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
#include "../smartfs.hpp"
#include "../shader.hpp"
#include "../D3.hpp"
#include "../coords.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

float width_w = 960.0f, height_w = 480.0f;

shader_t shader0, shader1, shader3;

coords_t  coords;
three3d_t three3D;

glm::vec3 viewPos;
glm::vec3 viewDir;
glm::mat4 transform;
glm::mat4 view;
glm::mat4 projection;

Light light;

void init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
//    ImGui::StyleColorsDark();
//    ImGui::StyleColorsLight();
    ImGui::StyleColorsClassic();
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();

    light.direction = glm::vec3(-1.f, -1.f, -1.f);
    light.ambient   = glm::vec3(1.f, 1.f, 1.f);
    light.diffuse   = glm::vec3(1.f, 1.f, 1.f);
    light.specular  = glm::vec3(1.f, 1.0f, 1.0f);

    viewPos = glm::vec3(-2.0f, 2.0f, -2.0f);
    viewDir = glm::vec3(0.0f, 0.0f, 0.0f);
    transform = glm::mat4(1.0f);
    view = glm::lookAt(viewPos, viewDir, glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(60.0f), width_w / height_w, 0.1f, 100.0f);

    smartfs::cache(smartfs::prefix(smartfs::runtime_path(), "gl3d"));
    shader0.init(
            smartfs::find("color_v.glsl").c_str(),
            smartfs::find("color_f.glsl").c_str());
    shader1.init(
            smartfs::find("v.shader").c_str(),
            smartfs::find("f.shader").c_str());
    shader3.init(
            smartfs::find("coords_v.glsl").c_str(),
            smartfs::find("coords_f.glsl").c_str());
    coords.init();
//    three3D.set_shader(shader0.ID, shader1.ID);
    // three3D.load3d(smartfs::find("cube.obj").c_str(), true);
   three3D.load3d(smartfs::find("Audi_R8_2017.obj"), shader0.ID, true);
    three3D.prepare();
}

void reshape(int width, int height) {
	std::cout << "window height_w : " << height_w << ", width_w : " << width_w << ", heigth : " << height << ", width : " << width << std::endl;
	width_w = (float)glutGet(GLUT_WINDOW_WIDTH);
	height_w = (float)glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, (int) width_w, (int) height_w);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glDepthFunc(GL_LESS);
	//std::cout << "window height : " << height_w << ", width : " << width_w << std::endl;
    view = glm::lookAt(viewPos, viewDir, glm::vec3(0.0f, 1.0f, 0.0f));
	width_w  = (float) glutGet(GLUT_WINDOW_WIDTH);
	height_w = (float) glutGet(GLUT_WINDOW_HEIGHT);
	glViewport(0, 0, (int) width_w, (int) height_w);
    projection = glm::perspective(glm::radians(60.0f), width_w / height_w, 0.1f, 100.0f);

    shader0.use();
    shader0.setMat4("transform", transform);
    shader0.setMat4("view", view);
    shader0.setMat4("projection", projection);
    shader0.setVec3("viewPos", viewPos);

    // shader0.setFloat("material.shininess", 2.0f);
    // shader0.setFloat("material.specular" , 2.0f);
    shader0.setVec3("light.direction"    , light.direction);
    shader0.setVec3("light.ambient"      , light.ambient);
    shader0.setVec3("light.diffuse"      , light.diffuse);
    shader0.setVec3("light.specular"     , light.specular);

    shader3.use();
    shader3.setMat4("transform", transform);
    shader3.setMat4("view", view);
    shader3.setMat4("projection", projection);
    shader3.setVec3("viewPos", viewPos);
    coords.render();

    shader1.use();
    shader1.setMat4("transform", transform);
    shader1.setMat4("view", view);
    shader1.setMat4("projection", projection);
    shader1.setVec3("viewPos", viewPos);
    shader1.setVec3("light.direction"    , light.direction);
    shader1.setVec3("light.ambient"      , light.ambient);
    shader1.setVec3("light.diffuse"      , light.diffuse);
    shader1.setVec3("light.specular"     , light.specular);

    three3D.render();
    glUseProgram(0);
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    ImGui::Begin("View Control");
    ImGui::SliderFloat("Eye X", &viewPos.x, -200.0f, 200.0f);
    ImGui::SliderFloat("Eye Y", &viewPos.y, -200.0f, 200.0f);
    ImGui::SliderFloat("Eye Z", &viewPos.z, -200.0f, 200.0f);
    ImGui::SliderFloat("Dir X", &viewDir.x, -200.0f, 200.0f);
    ImGui::SliderFloat("Dir Y", &viewDir.y, -200.0f, 200.0f);
    ImGui::SliderFloat("Dir Z", &viewDir.z, -200.0f, 200.0f);
    ImGui::SliderFloat("Light Dir X", &light.direction.x, -20.0f, 20.0f);
    ImGui::SliderFloat("Light Dir Y", &light.direction.y, -20.0f, 20.0f);
    ImGui::SliderFloat("Light Dir Z", &light.direction.z, -20.0f, 20.0f);
    // ImGui::SliderFloat("Coords line width", &coords.line_width, 0.01f, 10.f);
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
    glutInitWindowPosition(500, 0);
    glutCreateWindow("OpenGL Demo");
    glewInit();
	glutReshapeFunc(&reshape);
    glutDisplayFunc(display);
    
    init();

    glutMainLoop();

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    return 0;
}