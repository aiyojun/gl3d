#include <tuple>
#include <string>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#define ALL_IMPL
#include "../basic_ops.hpp"
#include "../kinematics.hpp"

float width_w = 960.0f, height_w = 480.0f;

std::vector<glm::vec3> points;
std::vector<std::tuple<glm::vec3, glm::vec3>> coordinates;
std::vector<RigidBody> body;
TF *timer;

void init()
{
    timer = new TF();
    coordinates = {
            std::tuple<glm::vec3, glm::vec3>(glm::vec3(0.f), glm::vec3(0.f)),
            std::tuple<glm::vec3, glm::vec3>(glm::vec3(0.f), glm::vec3(0.f)),};
    points = {
            glm::vec3(0.f), glm::vec3(2.f, 0.f, 0.f),
            glm::vec3(2.f, 0.f, 0.f), glm::vec3(4.f, 0.f, 0.f)
    };
    for (unsigned int i = 0; i < coordinates.size(); i++) {
        body.emplace_back(RigidBody(points[2 * i], std::get<0>(coordinates[i]), std::get<1>(coordinates[i])));
    }
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.f, width_w / height_w, 0.1f, 100.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT
    glColor3f(1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.f, width_w / height_w, 0.1f, 100.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
    TF& tf = *timer;
    float iTime = tf.iTime() * 1000 * 1.5;
    body[0].position.x = (float) cos(M_PI / 180 * iTime);
    body[0].position.y = (float) sin(M_PI / 180 * iTime);
    body[0].eular.z = iTime;
    body[1].eular.z = iTime;
    std::cout << "iTime : " << iTime
            << "\t (" << points[0].x << "," << points[0].y << "," << points[0].z << ") - (" << points[1].x << "," << points[1].y << "," << points[1].z
            << ") ### (" << points[2].x << "," << points[2].y << "," << points[2].z << ") - (" << points[3].x << "," << points[3].y << "," << points[3].z << ")"
            << std::endl;
    glm::mat4 accumulate(1.f);
    std::vector<glm::mat4> matrices;
    for (unsigned int i = 0; i < coordinates.size(); i++) {
        accumulate = accumulate * body[i].iState();
        matrices.emplace_back(accumulate);

        glm::vec3 p0 = glm::vec3(matrices[i] * glm::vec4(points[2 * i], 1.f)),
        p1 = glm::vec3(matrices[i] * glm::vec4(points[2 * i + 1], 1.f));

        glColor3f(0.8, 0.8, 0.8);
        glLineWidth(2);
        glBegin(GL_LINES);
        glVertex3f(p0.x, p0.y, p0.z);
        glVertex3f(p1.x, p1.y, p1.z);
        glEnd();

        glPointSize(5);
        glBegin(GL_POINTS);
        glVertex3f(0, 0, 0);
        glEnd();
    }

    glutSwapBuffers();
    glutPostRedisplay();
}

MAIN_BE
{
    int width = (int) width_w, height = (int) height_w;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH) - width, glutGet(GLUT_SCREEN_HEIGHT) - height);
    glutCreateWindow("OpenGL Demo");
    glewInit();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    init();

    glutMainLoop();
}
MAIN_EN