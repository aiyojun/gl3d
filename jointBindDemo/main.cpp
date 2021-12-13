#include <tuple>
#include <string>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <iomanip>
#include <algorithm>
#define ALL_IMPL
#include "../D3.hpp"
#include "../basic_type.h"
#include "../glm_helper.h"
#include "../smartfs.hpp"
#include "../shader.h"
#include "../basic_ops.hpp"
#include "../kinematics.hpp"
#include <imgui.h>
#include <imgui_impl_glut.h>
#include <imgui_impl_opengl2.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

#ifndef M_PI
#define M_PI 3.1415926
#endif

float width_w = 960.0f, height_w = 480.0f;
float offset_x = 0;
float offset_y = 0;
float offset_z = -10;
float alpha = 0;

ray_t light;

glm::vec3 
eular_head,
eular_body,
eular_left_arm,
eular_left_fore_arm,
eular_right_arm,
eular_right_fore_arm,
eular_left_leg,
eular_left_small_leg,
eular_right_leg,
eular_right_small_leg
;

std::vector<glm::mat4> cube_init_m;
std::vector<three3d_t> cubes;
three3d_t three;

Topology_t topo;

shader_t shader_j;

glm::vec3 viewDir;

void init_m() {
    eular_head = glm::vec3(0.f);
    eular_body = glm::vec3(0.f);
    eular_left_arm = glm::vec3(0.f);
    eular_left_fore_arm = glm::vec3(0.f);
    eular_right_arm = glm::vec3(0.f);
    eular_right_fore_arm = glm::vec3(0.f);
    eular_left_leg = glm::vec3(0.f);
    eular_left_small_leg = glm::vec3(0.f);
    eular_right_leg = glm::vec3(0.f);
    eular_right_small_leg = glm::vec3(0.f);
    // head
    glm::mat4 t0, t1, t2, t3, t4, t5, t6, t7, t8, t9;
    t0 = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 13.f, 0.f));
    t0 = t0 * glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, -0.5f, -0.5f));
    t1 = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 9.5f, 0.f));
    t1 = t1 * glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, -0.5f, -0.5f));
    t2 = glm::translate(glm::mat4(1.f), glm::vec3(-1.5f, 9.5f, 0.f));
    t2 = t2 * glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, -0.5f, -0.5f));
    t3 = glm::translate(glm::mat4(1.f), glm::vec3(-4.5f, 9.5f, 0.f));
    t3 = t3 * glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, -0.5f, -0.5f));
    t4 = glm::translate(glm::mat4(1.f), glm::vec3(1.5f, 9.5f, 0.f));
    t4 = t4 * glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, -0.5f, -0.5f));
    t5 = glm::translate(glm::mat4(1.f), glm::vec3(4.5f, 9.5f, 0.f));
    t5 = t5 * glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, -0.5f, -0.5f));
    t6 = glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, 5.5f, 0.f));
    t6 = t6 * glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, -0.5f, -0.5f));
    t7 = glm::translate(glm::mat4(1.f), glm::vec3(-1.5f, 2.f, 0.f));
    t7 = t7 * glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, -0.5f, -0.5f));
    t8 = glm::translate(glm::mat4(1.f), glm::vec3(0.5f, 5.5f, 0.f));
    t8 = t8 * glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, -0.5f, -0.5f));
    t9 = glm::translate(glm::mat4(1.f), glm::vec3(1.5f, 2.f, 0.f));
    t9 = t9 * glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, -0.5f, -0.5f));
    cube_init_m.emplace_back(t0);
    cube_init_m.emplace_back(t1);
    cube_init_m.emplace_back(t2);
    cube_init_m.emplace_back(t3);
    cube_init_m.emplace_back(t4);
    cube_init_m.emplace_back(t5);
    cube_init_m.emplace_back(t6);
    cube_init_m.emplace_back(t7);
    cube_init_m.emplace_back(t8);
    cube_init_m.emplace_back(t9);
}

void init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();

    /** TODO: init all preperations. */
    smartfs::cache(smartfs::prefix(smartfs::runtime_path(), "gl3d"));
    viewDir = glm::vec3(0.f);
    init_m();
    light.ia = 0.7f;
    light.id = 0.3f;
    light.is = 0.3f;
    light.dir = glm::vec3(0.f, -1.f, 0.f);
    light.color = glm::vec3(1.f, 1.f, 1.f);
    shader_j.init(smartfs::find("joint.glsl"));
    // std::cout << "-- 03" << std::endl;

   
    three.load3d(smartfs::find("cube.obj"), shader_j.shader_id, false);
    three.prepare();
    for (int i = 0; i < 10; i++) {
        three3d_t three3D;
        three3D.load3d(smartfs::find("cube.obj"), shader_j.shader_id, false);
        // std::cout << "-- 3.1" << std::endl;
        three3D.prepare();
        // std::cout << "-- 3.2" << std::endl;
        cubes.emplace_back(three3D);
    }
    // std::cout << "-- 3.5" << std::endl;
    topo = Topology_t::GenHuman();
    // std::cout << "-- 04" << std::endl;
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // gluPerspective(60.f, width_w / height_w, 0.1f, 100.f);
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    // gluLookAt(0, 7, 10, 0, 7, 0, 0, 1, 0);
}

void display()
{
    width_w  = (float) glutGet(GLUT_WINDOW_WIDTH);
    height_w = (float) glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, (int) width_w, (int) height_w);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glDepthFunc(GL_LESS);
    // glColor3f(1, 1, 1);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // gluPerspective(60.f, width_w / height_w, 0.1f, 100.f);
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    // gluLookAt(0, 7, 30, 0, 7, 0, 0, 1, 0);
    glm::vec3 viewPos = glm::vec3(offset_x, offset_y, offset_z);
    
    viewDir.x = viewPos.x + (float) (10.f * sin(2 * M_PI * alpha / 360));
    viewDir.z = viewPos.z + (float) (10.f * cos(2 * M_PI * alpha / 360));
    glm::mat4 view = glm::lookAt(viewPos, viewDir, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), width_w / height_w, 0.1f, 100.0f);
    shader_j.use();
    shader_j.set_mat4("view", view);
    shader_j.set_mat4("projection", projection);
    shader_j.set_vec3("viewPos", viewPos);
    shader_j.set_float("ray.ia", light.ia);
    shader_j.set_float("ray.id", light.id);
    shader_j.set_float("ray.is", light.is);
    shader_j.set_vec3("ray.dir", light.dir);
    shader_j.set_vec3("ray.color", light.color);

    // {
    //     shader_j.set_mat4("transform", glm::mat4(1.f));
    //     three.render();
    // }

    std::vector<glm::vec3> in = {
eular_head,
eular_body,
eular_left_arm,
eular_left_fore_arm,
eular_right_arm,
eular_right_fore_arm,
eular_left_leg,
eular_left_small_leg,
eular_right_leg,
eular_right_small_leg
    };
    std::vector<glm::mat4> mx = topo.getMatrices(in);
    for (unsigned int index = 0; index < 10; index++) {
        glm::mat4& m0 = cube_init_m[index];
        glm::mat4 transform = mx[index] * m0;
        // if (index == 9) {
        //     std::cout << "Final M : \n";
        //     glm::println(mx[index]);
        // }
        shader_j.use();
        shader_j.set_mat4("transform", transform);
        cubes[index].render();
    }

    glUseProgram(0);
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    ImGui::Begin("view");
    ImGui::SliderFloat("alpha", &alpha, 0.0f, 360.f);
    ImGui::SliderFloat("move x", &offset_x, -100.f, 100.f);
    ImGui::SliderFloat("move y", &offset_y, -100.f, 100.f);
    ImGui::SliderFloat("move z", &offset_z, -100.f, 100.f);
    ImGui::SliderFloat("Dir Y", &viewDir.y, -50.f, 50.f);
    ImGui::End();
    ImGui::Begin("control");
    ImGui::SliderFloat("head alpha", &eular_head.x, -360.f, 360.f);
    ImGui::SliderFloat("head beta", &eular_head.y, -360.f, 360.f);
    ImGui::SliderFloat("head gama", &eular_head.z, -360.f, 360.f);
    ImGui::SliderFloat("body alpha", &eular_body.x, -360.f, 360.f);
    ImGui::SliderFloat("body beta", &eular_body.y, -360.f, 360.f);
    ImGui::SliderFloat("body gama", &eular_body.z, -360.f, 360.f);
    ImGui::SliderFloat("left_arm alpha", &eular_left_arm.x, -360.f, 360.f);
    ImGui::SliderFloat("left_arm beta", &eular_left_arm.y, -360.f, 360.f);
    ImGui::SliderFloat("left_arm gama", &eular_left_arm.z, -360.f, 360.f);
    ImGui::SliderFloat("left_fore_arm alpha", &eular_left_fore_arm.x, -360.f, 360.f);
    ImGui::SliderFloat("left_fore_arm beta", &eular_left_fore_arm.y, -360.f, 360.f);
    ImGui::SliderFloat("left_fore_arm gama", &eular_left_fore_arm.z, -360.f, 360.f);
    ImGui::SliderFloat("right_arm alpha", &eular_right_arm.x, -360.f, 360.f);
    ImGui::SliderFloat("right_arm beta", &eular_right_arm.y, -360.f, 360.f);
    ImGui::SliderFloat("right_arm gama", &eular_right_arm.z, -360.f, 360.f);
    ImGui::SliderFloat("right_fore_arm alpha", &eular_right_fore_arm.x, -360.f, 360.f);
    ImGui::SliderFloat("right_fore_arm beta", &eular_right_fore_arm.y, -360.f, 360.f);
    ImGui::SliderFloat("right_fore_arm gama", &eular_right_fore_arm.z, -360.f, 360.f);
    ImGui::SliderFloat("left_leg alpha", &eular_left_leg.x, -360.f, 360.f);
    ImGui::SliderFloat("left_leg beta", &eular_left_leg.y, -360.f, 360.f);
    ImGui::SliderFloat("left_leg gama", &eular_left_leg.z, -360.f, 360.f);
    ImGui::SliderFloat("left_small_leg alpha", &eular_left_small_leg.x, -360.f, 360.f);
    ImGui::SliderFloat("left_small_leg beta", &eular_left_small_leg.y, -360.f, 360.f);
    ImGui::SliderFloat("left_small_leg gama", &eular_left_small_leg.z, -360.f, 360.f);
    ImGui::SliderFloat("right_leg alpha", &eular_right_leg.x, -360.f, 360.f);
    ImGui::SliderFloat("right_leg beta", &eular_right_leg.y, -360.f, 360.f);
    ImGui::SliderFloat("right_leg gama", &eular_right_leg.z, -360.f, 360.f);
    ImGui::SliderFloat("right_small_leg alpha", &eular_right_small_leg.x, -360.f, 360.f);
    ImGui::SliderFloat("right_small_leg beta", &eular_right_small_leg.y, -360.f, 360.f);
    ImGui::SliderFloat("right_small_leg gama", &eular_right_small_leg.z, -360.f, 360.f);

    ImGui::End();
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers();
    glutPostRedisplay();
}

MAIN_BE
{
    int width = (int) width_w, height = (int) height_w;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH) - width, glutGet(GLUT_SCREEN_HEIGHT) - height);
    glutCreateWindow("OpenGL Demo");
    glewInit();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    init();

    glutMainLoop();

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();
}
MAIN_EN