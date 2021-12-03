#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

typedef void (*Function)();
typedef void (*Function2)(int, int);
typedef void (*Function3)(int, int, int, int);

class widget_t {
public:
    void init(
        int* argc, char** argv,
        int width, int height, int pos_x, int pos_y,
        const char* window_name,
        Function display, Function2 reshape,
        Function3 mouse = nullptr, Function3 mouseWheel = nullptr);
    void flush();
    void loop();
};

#if defined(WIDGET_IMPL) || defined(ALL_IMPL)

void widget_t::init(
    int* argc, char** argv,
    int width, int height,
    int pos_x, int pos_y,
    const char* window_name,
    Function display,
    Function2 reshape,
    Function3 mouse = nullptr,
    Function3 mouseWheel = nullptr)
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(pos_x, pos_y);
    glutCreateWindow(window_name);
    glewInit();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    if (mouse != nullptr)
        glutMouseFunc(mouse);
    if (mouseWheel != nullptr)
        glutMouseWheelFunc(mouseWheel);
}

void widget_t::flush() 
{
    glutSwapBuffers();
    glutPostRedisplay();
}

inline void widget_t::loop()
{glutMainLoop();}

#endif