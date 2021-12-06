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

float width_w = 960.0f, height_w = 480.0f;

shader_t shader0, shader1, shader3, shader_sky;

coords_t  coords;
three3d_t three3D, sky_box;

unsigned int sky_tex = 0;

glm::vec3 viewPos;
glm::vec3 viewDir;
glm::mat4 transform;
glm::mat4 view;
glm::mat4 projection;

glm::mat4 transform_sky;

Light light;

unsigned int create_sky(const char* prefix) // prefix = F:/gl3d-main/asset/default_sky_box_
{
    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

    unsigned char *side_d, *sky_d, *ground_d;
    int width, height, depth;
    side_d   = stbi_load((std::string(prefix) + "side.jpg"  ).c_str(), &width, &height, &depth, 0);
    sky_d    = stbi_load((std::string(prefix) + "sky.jpg"   ).c_str(), &width, &height, &depth, 0);
    ground_d = stbi_load((std::string(prefix) + "ground.jpg").c_str(), &width, &height, &depth, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 
    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, side_d);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 
    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, side_d);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 
    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, side_d);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 
    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, side_d);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 
    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, sky_d);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 
    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ground_d);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    stbi_image_free((void *) side_d);
    stbi_image_free((void *) sky_d);
    stbi_image_free((void *) ground_d);

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

    viewPos = glm::vec3(-2.0f, 2.0f, -2.0f);
    viewDir = glm::vec3(0.0f, 0.0f, 0.0f);
    transform = glm::mat4(1.0f);
    view = glm::lookAt(viewPos, viewDir, glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(60.0f), width_w / height_w, 0.1f, 100.0f);

    shader0.init("../../shader/color_v.glsl", "../../shader/color_f.glsl");
    shader1.init("../../shader/v.shader", "../../shader/f.shader");
    shader3.init("../../shader/coords_v.glsl", "../../shader/coords_f.glsl");
    shader_sky.init("../../shader/sky_box_v.glsl", "../../shader/sky_box_f.glsl");
    coords.init();
    three3D.set_shader(shader0.ID, shader1.ID);
    three3D.load3d("../../asset/cube.obj", true);
//    three3D.load3d("../asset/uploads_files_2787791_Mercedes+Benz+GLS+580.obj", false);
    three3D.prepare();

    sky_box.set_shader(shader_sky.ID);
    sky_box.no_fragment_variable = true;
    sky_box.attribute_n = 1;
    sky_box.attributes_m = {3};
    sky_box.set_not_normal();
    sky_box.load3d("../../asset/cube.obj", false);
    sky_box.prepare();
    sky_tex = create_sky("../../asset/default_sky_box_");
    std::cout << "-- create sky box texture id : " << sky_tex << std::endl;
    shader_sky.use();
    shader_sky.setInt("skybox", 0);
    // glUniform1i(glGetUniformLocation(shader_sky.ID, "skybox"), 0);

    // shader0.use();
    // shader1.use();

}

void reshape(int width, int height) {
	std::cout << "window height_w : " << height_w << ", width_w : " << width_w << ", heigth : " << height << ", width : " << width << std::endl;
	width_w = (float)glutGet(GLUT_WINDOW_WIDTH);
	height_w = (float)glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, width_w, height_w);
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

    transform_sky = glm::translate(transform_sky, glm::vec3(-0.5f, -0.5f, -0.5f));
    // transform_sky = glm::scale(transform_sky, glm::vec3(10.f, 10.f, 10.f));
    // transform_sky = glm::translate(transform_sky, viewPos);



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

    //three3D.render();

    // sky box transform
    // glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    shader_sky.use();
    // shader_sky.setMat4("view", glm::mat4(glm::mat3(view)));
    shader_sky.setMat4("transform", transform_sky);
    shader_sky.setMat4("projection", projection);
    shader_sky.setMat4("view", view);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, sky_tex);
    sky_box.render();
    glDepthFunc(GL_LESS);
    // glDepthMask(GL_TRUE);

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