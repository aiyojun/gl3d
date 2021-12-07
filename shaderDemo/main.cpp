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
#include "../basic_ops.hpp"
#include "../shader.h"
#include "../D3.hpp"
#include "../coords.hpp"
#include "../skybox.hpp"
#include "../smartfs.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

#define _PI_ 3.1415926
float width_w = 960.0f, height_w = 480.0f;
shader_t shader_ray;
shader_t shader_sky;
ray_t light;
skybox_t skybox; unsigned int sky_tex = 0;
three3d_t three3D;
static ImVec4 clear_color, clear_color_kd, clear_color_ks;
bool use_default = true;
float alpha = 3.1415926;
float offset_x = 0;
float offset_y = 1;
float offset_z = -8;

glm::vec3 viewPos;
glm::vec3 viewDir;
glm::mat4 view;
glm::mat4 transform;
glm::mat4 projection;
glm::mat4 transform_sky;


shader_t shader_co;
coords_t  coords;

glm::vec3 gravity_point = glm::vec3(0.f);
float spindle_alpha = 0.0f;
float spindle_speed = 0.025f;
float last_angle = 0.f;
long last_time = 0l;
long begin_time = 0l;
glm::vec2 spindle = glm::vec2(0.f);

int rotate_factor = 1;

void button_use() {use_default = !use_default;}
void button_switch_rotate() {rotate_factor = rotate_factor ? 0 : 1;}


void init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();

    smartfs::cache(smartfs::prefix(smartfs::runtime_path(), "gl3d"));
    shader_ray.init(smartfs::find("onelight.glsl"));

    light.ia = 0.3f;
    light.id = 0.3f;
    light.is = 0.3f;
    light.dir = glm::vec3(0.f, -1.f, 0.f);
    light.color = glm::vec3(1.f, 1.f, 1.f);
    clear_color = ImVec4(0.9f, 0.9f, 0.9f, 1.00f);
    clear_color_kd = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);
    clear_color_ks = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

    skybox.prepare();
    std::vector<std::string> skys = {
            smartfs::find("default_sky_box_side.jpg"),
            smartfs::find("default_sky_box_side.jpg"),
            smartfs::find("default_sky_box_sky.jpg"),
            smartfs::find("default_sky_box_ground.jpg"),
            smartfs::find("default_sky_box_side.jpg"),
            smartfs::find("default_sky_box_side.jpg")
    };
    sky_tex = skybox_t::create_sky(skys);
    shader_sky.init(smartfs::find("skybox.glsl"));
    shader_sky.use();
    shader_sky.set_int("skybox", 0);

    shader_co.init(smartfs::find("coords.glsl"));
    coords.init();

    three3D.load3d(smartfs::find("Audi_R8_2017.obj"), shader_ray.shader_id, false);
    three3D.prepare();
    begin_time = jlib::GetSystemCurrentMs();
    last_time = begin_time;
    spindle.x = (three3D.edge_min.x + three3D.edge_max.x) / 2;
    spindle.y = (three3D.edge_min.z + three3D.edge_max.z) / 2;
    gravity_point.x = spindle.x;
    gravity_point.y = (three3D.edge_min.y + three3D.edge_max.y) / 2;
    gravity_point.z = spindle.y;
}


void reshape(int width, int height) {
    std::cout << "window height_w : " << height_w << ", width_w : " << width_w << ", heigth : " << height << ", width : " << width << std::endl;
    width_w  = (float)glutGet(GLUT_WINDOW_WIDTH);
    height_w = (float)glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, (int) width_w, (int) height_w);
}

float scale_c = 100.f;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glDepthFunc(GL_LESS);
    long now = jlib::GetSystemCurrentMs();
    spindle_alpha = last_angle + (float) ((double) (now - last_time) / 1000 * 360.f * spindle_speed * rotate_factor);
    while (spindle_alpha >= 360.f) spindle_alpha = (float) (spindle_alpha - 360.f);
    last_angle = spindle_alpha;
    last_time = now;

    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(-gravity_point.x, -gravity_point.y, -gravity_point.z));
    transform = glm::rotate(transform, glm::radians(spindle_alpha), glm::vec3(0.f, 1.0f, 0.0f));
    viewPos = glm::vec3(offset_x, offset_y, offset_z);
    viewDir.x = viewPos.x + (float) (10.f * sin(2 * _PI_ * alpha / 360));
    viewDir.z = viewPos.z + (float) (10.f * cos(2 * _PI_ * alpha / 360));
    view = glm::lookAt(viewPos, viewDir, glm::vec3(0.0f, 1.0f, 0.0f));
    // view = glm::rotate(view, glm::radians(alpha), glm::vec3(0.f, 1.f, 0.f));
    projection = glm::perspective(glm::radians(60.0f), width_w / height_w, 0.1f, 100.0f);
    width_w  = (float) glutGet(GLUT_WINDOW_WIDTH);
    height_w = (float) glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, (int) width_w, (int) height_w);

    shader_co.use();
    shader_co.set_mat4("view", view);
    shader_co.set_mat4("transform", transform);
    shader_co.set_mat4("projection", projection);
    shader_co.set_vec3("viewPos", viewPos);
    coords.render();

    shader_ray.use();
    shader_ray.set_float("ray.ia", light.ia);
    shader_ray.set_float("ray.id", light.id);
    shader_ray.set_float("ray.is", light.is);
    shader_ray.set_vec3("ray.dir", light.dir);
    shader_ray.set_vec3("ray.color", light.color);
    shader_ray.set_mat4("view", view);
    shader_ray.set_mat4("transform", transform);
    shader_ray.set_mat4("projection", projection);
    shader_ray.set_vec3("viewPos", viewPos);
    if (use_default) {
        float d = 1.f, s = 1.f;
        shader_ray.set_vec3("material.Ka", glm::vec3(clear_color.x, clear_color.y, clear_color.z));
        shader_ray.set_vec3("material.Kd", glm::vec3(d * clear_color_kd.x, d * clear_color_kd.y, d * clear_color_kd.z));
        shader_ray.set_vec3("material.Ks", glm::vec3(s * clear_color_ks.x, s * clear_color_ks.y, s * clear_color_ks.z));
    }
    three3D.render();

    transform_sky = glm::mat4(1.0f);
    transform_sky = glm::scale(transform_sky, glm::vec3(60.f, 60.f, 60.f));
    transform_sky[3][0] = transform_sky[3][0] + viewPos.x;
    transform_sky[3][1] = transform_sky[3][1] + viewPos.y;
    transform_sky[3][2] = transform_sky[3][2] + viewPos.z;

    glDepthFunc(GL_LEQUAL);
    shader_sky.use();
    shader_sky.set_mat4("transform", transform_sky);
    shader_sky.set_mat4("projection", projection);
    shader_sky.set_mat4("view", view);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, sky_tex);
    skybox.render();
    glDepthFunc(GL_LESS);

    glUseProgram(0);
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    ImGui::Begin("View Control");
    ImGui::SliderFloat("speed", &spindle_speed, 0.01f, 1.0f);
    ImGui::SliderFloat("alpha", &alpha, 0.0f, 360.f);
    ImGui::SliderFloat("move x", &offset_x, -100.f, 100.f);
    ImGui::SliderFloat("move y", &offset_y, -100.f, 100.f);
    ImGui::SliderFloat("move z", &offset_z, -100.f, 100.f);
    // ImGui::SliderFloat("scale", &scale_c, -200.f, 200.f);
    // ImGui::SliderFloat("Eye X", &viewPos.x, scale_c * -1.f, scale_c);
    // ImGui::SliderFloat("Eye Y", &viewPos.y, scale_c * -1.f, scale_c);
    // ImGui::SliderFloat("Eye Z", &viewPos.z, scale_c * -1.f, scale_c);
    // ImGui::SliderFloat("Dir X", &viewDir.x, scale_c * -1.f, scale_c);
    ImGui::SliderFloat("Dir Y", &viewDir.y, scale_c * -1.f, scale_c);
    ImGui::ColorEdit3("ambient", (float*)&clear_color);
    ImGui::ColorEdit3("diffuse", (float*)&clear_color_kd);
    ImGui::ColorEdit3("specular", (float*)&clear_color_ks);
    if (ImGui::Button("Switch Material")) button_use();
    ImGui::InputText("Default Material: ", (char *) (use_default ? "yes" : "no "), 3);
    if (ImGui::Button("Switch Rotate")) button_switch_rotate();
    ImGui::InputText("Rotate state: ", (char *) (use_default ? "yes" : "no "), 3);
    // ImGui::SliderFloat("Dir Z", &viewDir.z, scale_c * -1.f, scale_c);
     ImGui::SliderFloat("light x", &light.dir.x, -2.0f, 2.0f);
     ImGui::SliderFloat("light y", &light.dir.y, -2.0f, 2.0f);
     ImGui::SliderFloat("light z", &light.dir.z, -2.0f, 2.0f);
     ImGui::SliderFloat("light ia", &light.ia, 0.0f, 1.0f);
     ImGui::SliderFloat("light id", &light.id, 0.0f, 1.0f);
     ImGui::SliderFloat("light is", &light.is, 0.0f, 1.0f);
    ImGui::SliderFloat("Coords Width", &coords.line_width, 0.001f, 10.f);
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
    glutInitWindowPosition(500, 000);
    glutCreateWindow("OpenGL Demo");
    glewInit();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    init();

    glutMainLoop();

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    return 0;
}