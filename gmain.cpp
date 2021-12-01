#include "window.h"
#include "model3d.h"
#include "extensions.h"
#include "shader.h"
#include "drawing.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

j_model3d model;
j_window window;
j_imgui  widget;
Shader   shader;
Camera   camera;
j_line   x_scope, y_scope, z_scope, l_scope;
j_cylinder_cone   cone;
j_cylinder   cylinder;
j_face   face0;
static ImVec4 clear_color;

void init() {
    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    model.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    model.rotate = glm::vec3(0.0f, 0.0f, 0.0f);

    camera._near = 0.1f;
    camera._far = 100.0f;
    camera.w_h = 960.0f / 480.0f;
    camera._degree = 60.0f;

//    camera.moving = glm::vec3(0.0f);

    camera.pos    = glm::vec3(0.0f, 0.0f, -10.0f);
    camera.dir    = glm::vec3(0.0f, 0.0f, 0.0f);
    camera.cap    = glm::vec3(0.0f, 1.0f, 0.0f);
}

void resetMatrix() {
    model.transform = glm::mat4(1.0f);
    model.transform = glm::translate(model.transform, model.pos);
    model.transform = glm::rotate(model.transform, glm::radians(model.rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model.transform = glm::rotate(model.transform, glm::radians(model.rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model.transform = glm::rotate(model.transform, glm::radians(model.rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));

    camera.view = glm::lookAt(camera.pos, camera.dir, camera.cap);
//    glm::vec3 walk_dir = camera.dir - camera.pos;
//    walk_dir = glm::normalize(walk_dir);
//    camera.view = glm::translate(camera.view, walk_dir * glm::vec3(0.1));

    camera.projection = glm::perspective(glm::radians(camera._degree), camera.w_h, camera._near, camera._far);

    shader.setMat4("transform", model.transform);
    shader.setMat4("view", camera.view);
    shader.setMat4("projection", camera.projection);

    shader.setVec3("viewPos", camera.pos);
//    shader.setVec3("lightPos", )
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

void display() {
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    shader.use();
    resetMatrix();

    x_scope.draw();
    y_scope.draw();
    z_scope.draw();
    cone.draw();
    cylinder.draw();
//      face0.draw();
//    l_scope.draw();

    model.draw(shader);

    glUseProgram(0);
    widget.beforeDraw();
    ImGui::Begin("View Control");
    ImGui::SliderFloat("Eye X", &camera.pos.x, -20.0f, 20.0f);
    ImGui::SliderFloat("Eye Y", &camera.pos.y, -20.0f, 20.0f);
    ImGui::SliderFloat("Eye Z", &camera.pos.z, -20.0f, 20.0f);
    ImGui::SliderFloat("Dir X", &camera.dir.x, -20.0f, 20.0f);
    ImGui::SliderFloat("Dir Y", &camera.dir.y, -20.0f, 20.0f);
    ImGui::SliderFloat("Dir Z", &camera.dir.z, -20.0f, 20.0f);
    ImGui::ColorEdit3("Background Color", (float*)&clear_color);
    ImGui::End();
    widget.afterDraw();

    window.flush();
}

//void mouseWheel(int button, int dir, int x, int y) {
//    if (dir > 0) {
//        std::cout << "zoom in" << std::endl;
//        camera.view = glm::lookAt(camera.pos, camera.dir, camera.cap);
//        glm::vec3 walk_dir = camera.dir - camera.pos;
//        camera.view = glm::translate(camera.view, walk_dir * glm::vec3(0.01));
//    } else if (dir < 0) {
//        std::cout << "zoom out" << std::endl;
//        camera.view = glm::lookAt(camera.pos, camera.dir, camera.cap);
//        glm::vec3 walk_dir = camera.dir - camera.pos;
//        camera.view = glm::translate(camera.view, walk_dir * glm::vec3(-0.01));
//    }
//}

int main(int argc, char** argv) {
//     model.load("/opt/lea/learn-gl/uploads_files_2787791_Mercedes+Benz+GLS+580.obj");
     model.load("/opt/lea/learn-gl/copygls.obj");
     window.init(&argc, argv, 960, 480, 400, 400, "OpenGL GLC", display, reshape);
    shader.init("/opt/jpro/gl3d/shader/comm_vertex.shader",
//                "/opt/jpro/gl3d/shader/entity_fragment.shader");
                "/opt/jpro/gl3d/shader/comm_fragment.shader");
           init();
    widget.init();
     model.init("/opt/lea/learn-gl");
   x_scope.init(4, glm::vec3(0.0f),glm::vec3(10.0f, 0.0f, 0.0f),glm::vec3(1.0f, 0.0f, 0.0f));
   y_scope.init(4, glm::vec3(0.0f),glm::vec3(0.0f, 10.0f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f));
   z_scope.init(4, glm::vec3(0.0f),glm::vec3(0.0f, 0.0f, 10.0f),glm::vec3(0.0f, 0.0f, 1.0f));
   l_scope.init(4, glm::vec3(-10.0f, -10.0f, -10.0f), glm::vec3(10.0f, 10.0f, 10.0f),glm::vec3(0.0f, 0.0f, 1.0f));
     face0.init();
      cone.init(glm::vec3(0.0f, 10.0f, 0.0f), 0.35f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
//  cylinder.init(glm::vec3(0.0f, 0.0f, 0.0f), 2.0f, 4.0f, glm::vec3(1.0f, 1.0f, 1.0f));
  cylinder.init(glm::vec3(0.0f, 0.0f, 0.0f), 0.15f, 10.0f, glm::vec3(1.0f, 1.0f, 1.0f));
//  cylinder.init(glm::vec3(0.0f, 0.0f, 0.0f), 0.2f, 10.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    window.loop();

    return 0;
}