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
#include "../coords.hpp"

#define _PI_ 3.1415926

float width_w = 960.0f, height_w = 480.0f;

unsigned int sky_tex = 0;
shader_t shader3, shader_sky;
glm::mat4 transform_sky;

coords_t  coords;
// three3d_t sky_box;

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

unsigned int skyboxVAO = 0, skyboxVBO = 0;

float alpha = 0;
float offset_x = 0;
float offset_y = 0;
float offset_z = 0;
glm::vec3 viewRot;
glm::vec3 viewPos;
glm::vec3 viewDir;
glm::mat4 transform;
glm::mat4 view;
glm::mat4 projection;

Light light;

unsigned int create_sky(const char* prefix) // prefix = F:/gl3d-main/asset/default_sky_box_
{
    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

    unsigned char *right, *left, *top, *bottom, *front, *back;//*side_d, *sky_d, *ground_d, 
    int width, height, depth;
    std::string group[] = {
        "default_sky_box_side.jpg", 
        "default_sky_box_side.jpg", 
        "default_sky_box_sky.jpg", 
        "default_sky_box_ground.jpg",
        "default_sky_box_side.jpg", 
        "default_sky_box_side.jpg"
        };
    right    = stbi_load((std::string(prefix) + group[0]).c_str(), &width, &height, &depth, 0);
    left     = stbi_load((std::string(prefix) + group[1]).c_str(), &width, &height, &depth, 0);
    top      = stbi_load((std::string(prefix) + group[2]).c_str(), &width, &height, &depth, 0);
    bottom   = stbi_load((std::string(prefix) + group[3]).c_str(), &width, &height, &depth, 0);
    front    = stbi_load((std::string(prefix) + group[4]).c_str(), &width, &height, &depth, 0);
    back     = stbi_load((std::string(prefix) + group[5]).c_str(), &width, &height, &depth, 0);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, right);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, left);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, front);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, back);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, top);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bottom);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    stbi_image_free((void *)right);
    stbi_image_free((void *)left);
    stbi_image_free((void *)top);
	stbi_image_free((void *)bottom);
	stbi_image_free((void *)front);
	stbi_image_free((void *)back);

    return tex;
}

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

    light.direction = glm::vec3(-1.f, -1.f, -1.f);
    light.ambient   = glm::vec3(1.f, 1.f, 1.f);
    light.diffuse   = glm::vec3(1.f, 1.f, 1.f);
    light.specular  = glm::vec3(1.f, 1.0f, 1.0f);

    // viewPos = glm::vec3(2.0f + offset_x, 2.0f, 0.0f + offset_z);
    // viewDir = glm::vec3(2.0f + offset_x, 2.0f, 10.0f + offset_z);
    // viewRot = glm::vec3(1.f, 1.f, 1.f);
    // viewPos = glm::vec3(-2.0f, 0.0f, -2.0f);
    // viewDir = glm::vec3(0.0f, 0.0f, 0.0f);
    transform = glm::mat4(1.0f);
    //view = glm::lookAt(viewPos, viewDir, glm::vec3(0.0f, 1.0f, 0.0f));
    //projection = glm::perspective(glm::radians(60.0f), width_w / height_w, 0.1f, 100.0f);

    shader3.init("../shader/coords_v.glsl", "../shader/coords_f.glsl");
    shader_sky.init("../shader/sky_box_v.glsl", "../shader/sky_box_f.glsl");
    coords.init();
    // sky_box.set_shader(shader_sky.ID);
    // sky_box.no_fragment_variable = true;
    // sky_box.attribute_n = 1;
    // sky_box.attributes_m = {3};
    // sky_box.set_not_normal();
    // sky_box.load3d("../../asset/cube.obj", false);
    // sky_box.prepare();

    
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    sky_tex = create_sky("../asset/");
    std::cout << "-- create sky box texture id : " << sky_tex << std::endl;
    shader_sky.use();
    shader_sky.setInt("skybox", 0);
}

void reshape(int width, int height) {
	std::cout << "window height_w : " << height_w << ", width_w : " << width_w << ", heigth : " << height << ", width : " << width << std::endl;
	width_w = (float)glutGet(GLUT_WINDOW_WIDTH);
	height_w = (float)glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, width_w, height_w);
}

float scale_c = 100.f;
float scale_p = 2.f;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glDepthFunc(GL_LESS);
    viewPos = glm::vec3(offset_x, offset_y, offset_z);
	// std::cout << "window height : " << height_w << ", width : " << width_w << std::endl;
    viewDir.x = viewPos.x + 10 * sin(2 * _PI_ * alpha / 360);
    viewDir.z = viewPos.z + 10 * cos(2 * _PI_ * alpha / 360);
    view = glm::lookAt(viewPos, viewDir, glm::vec3(0.0f, 1.0f, 0.0f));
    // view = glm::rotate(view, glm::radians(alpha), glm::vec3(0.f, 1.f, 0.f));
    projection = glm::perspective(glm::radians(60.0f), width_w / height_w, 0.1f, 100.0f);
	width_w  = (float) glutGet(GLUT_WINDOW_WIDTH);
	height_w = (float) glutGet(GLUT_WINDOW_HEIGHT);
	glViewport(0, 0, (int) width_w, (int) height_w);
    

    transform_sky = glm::mat4(1.0f);
    transform_sky = glm::scale(transform_sky, glm::vec3(scale_p, scale_p, scale_p));
    // transform_sky = glm::translate(transform_sky, glm::vec3(-0.5f, - 0.5f, -0.5f));
    transform_sky[3][0] = transform_sky[3][0] + viewPos.x;
    transform_sky[3][1] = transform_sky[3][1] + viewPos.y;
    transform_sky[3][2] = transform_sky[3][2] + viewPos.z;
    // transform_sky = glm::translate(transform_sky, glm::vec3(0, -0.5f * scale_p, 0));
    // transform_sky = glm::translate(transform_sky, glm::vec3(0, 0, -0.5f * scale_p));


    // transform_sky = glm::translate(transform_sky, viewPos);
    shader3.use();
    shader3.setMat4("view", view);
    shader3.setMat4("transform", transform);
    shader3.setMat4("projection", projection);
    shader3.setVec3("viewPos", viewPos);
    coords.render();

    //three3D.render();

    // sky box transform
    // glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    shader_sky.use();
    // shader_sky.setMat4("view", glm::mat4(glm::mat3(view)));
    shader_sky.setMat4("transform", transform_sky);
    shader_sky.setMat4("projection", projection);
    shader_sky.setMat4("view", view);
    // sky_box.render();
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, sky_tex);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
    // glDepthMask(GL_TRUE);

    glUseProgram(0);
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    ImGui::Begin("View Control");
    ImGui::SliderFloat("alpha", &alpha, 0.0f, 1000.f);
    ImGui::SliderFloat("sky", &scale_p, 0.1f, 100.f);
    ImGui::SliderFloat("move x", &offset_x, -100.f, 100.f);
    ImGui::SliderFloat("move y", &offset_y, -100.f, 100.f);
    ImGui::SliderFloat("move z", &offset_z, -100.f, 100.f);
    // ImGui::SliderFloat("scale", &scale_c, -200.f, 200.f);
    // ImGui::SliderFloat("Eye X", &viewPos.x, scale_c * -1.f, scale_c);
    // ImGui::SliderFloat("Eye Y", &viewPos.y, scale_c * -1.f, scale_c);
    // ImGui::SliderFloat("Eye Z", &viewPos.z, scale_c * -1.f, scale_c);
    // ImGui::SliderFloat("Dir X", &viewDir.x, scale_c * -1.f, scale_c);
    ImGui::SliderFloat("Dir Y", &viewDir.y, scale_c * -1.f, scale_c);
    // ImGui::SliderFloat("Dir Z", &viewDir.z, scale_c * -1.f, scale_c);
    // ImGui::SliderFloat("Light Dir X", &light.direction.x, -20.0f, 20.0f);
    // ImGui::SliderFloat("Light Dir Y", &light.direction.y, -20.0f, 20.0f);
    // ImGui::SliderFloat("Light Dir Z", &light.direction.z, -20.0f, 20.0f);
    ImGui::SliderFloat("Coords line width", &coords.line_width, 0.01f, 10.f);
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

	glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    init();

    glutMainLoop();

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    return 0;
}