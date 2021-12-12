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
#include "../basic_ops.hpp"
#include "../kinematics.hpp"
#include <imgui.h>
#include <imgui_impl_glut.h>
#include <imgui_impl_opengl2.h>

#ifndef M_PI
#define M_PI 3.1415926
#endif

float width_w = 960.0f, height_w = 480.0f;

struct triple_t {
    glm::vec3 bas, end, pos, rot;
};

std::vector<glm::vec3> points;
std::vector<std::tuple<glm::vec3, glm::vec3>> coordinates;
std::vector<RigidBody> body;
TF *timer;
Topology topology;
// std::tuple<glm::vec3, glm::vec3, glm::vec3>  
triple_t 
_head, 
_body, 
_leftArm, 
_leftForeArm, 
_rightArm, 
_rightForeArm, 
_leftBigLeg, 
_leftSmallLeg, 
_rightBigLeg, 
_rightSmallLeg;
std::vector<triple_t*> parts;
std::vector<std::string> uids;

class vertex_tt {
public:
    glm::vec3 pos;
    int mat_i;
    vertex_tt(glm::vec3 v, int i);
};

vertex_tt::vertex_tt(glm::vec3 v, int i): pos(std::move(v)), mat_i(i) {}

std::vector<vertex_tt> person_points;

void init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();

    uids = {"head", "body", "leftArm", "leftForeArm", "rightArm", "rightForeArm", "leftBigLeg", "leftSmallLeg", "rightBigLeg", "rightSmallLeg"};

    // person_points.emplace_back(vertex_tt(glm::vec3( 0.f, 14.f, 0.f), 0)); person_points.emplace_back(vertex_tt(glm::vec3( 0.f, 12.f, 0.f), 0)); 
    // person_points.emplace_back(vertex_tt(glm::vec3( 0.f, 12.f, 0.f), 1)); person_points.emplace_back(vertex_tt(glm::vec3( 0.f,  7.f, 0.f), 1));  // body
    // person_points.emplace_back(vertex_tt(glm::vec3( 0.f, 12.f, 0.f), 2)); person_points.emplace_back(vertex_tt(glm::vec3(-3.f, 12.f, 0.f), 2)); 
    // person_points.emplace_back(vertex_tt(glm::vec3(-3.f, 12.f, 0.f), 3)); person_points.emplace_back(vertex_tt(glm::vec3(-6.f, 12.f, 0.f), 3)); 
    // person_points.emplace_back(vertex_tt(glm::vec3( 0.f, 12.f, 0.f), 4)); person_points.emplace_back(vertex_tt(glm::vec3( 3.f, 12.f, 0.f), 4)); 
    // person_points.emplace_back(vertex_tt(glm::vec3( 3.f, 12.f, 0.f), 5)); person_points.emplace_back(vertex_tt(glm::vec3( 6.f, 12.f, 0.f), 5)); 
    // person_points.emplace_back(vertex_tt(glm::vec3( 0.f,  7.f, 0.f), 6)); person_points.emplace_back(vertex_tt(glm::vec3(-1.f,  4.f, 0.f), 6));  // left  big leg
    // person_points.emplace_back(vertex_tt(glm::vec3(-1.f,  4.f, 0.f), 7)); person_points.emplace_back(vertex_tt(glm::vec3(-2.f,  0.f, 0.f), 7));  // left small leg
    // person_points.emplace_back(vertex_tt(glm::vec3( 0.f,  7.f, 0.f), 8)); person_points.emplace_back(vertex_tt(glm::vec3( 1.f,  4.f, 0.f), 8)); 
    // person_points.emplace_back(vertex_tt(glm::vec3( 1.f,  4.f, 0.f), 9)); person_points.emplace_back(vertex_tt(glm::vec3( 2.f,  0.f, 0.f), 9));

    // timer = new TF();
    // coordinates = {
    //         std::tuple<glm::vec3, glm::vec3>(glm::vec3(0.f), glm::vec3(0.f)),
    //         std::tuple<glm::vec3, glm::vec3>(glm::vec3(0.f), glm::vec3(0.f)),};
    // points = {
    //         glm::vec3(0.f), glm::vec3(2.f, 0.f, 0.f),
    //         glm::vec3(2.f, 0.f, 0.f), glm::vec3(4.f, 0.f, 0.f)
    // };
    // for (unsigned int i = 0; i < coordinates.size(); i++) {
    //     body.emplace_back(RigidBody(points[2 * i], std::get<0>(coordinates[i]), std::get<1>(coordinates[i])));
    // }

	_head.bas          = glm::vec3( 0.f, 14.f, 0.f);
    _body.bas          = glm::vec3( 0.f, 12.f, 0.f);
    _leftArm.bas       = glm::vec3( 0.f, 12.f, 0.f);
    _leftForeArm.bas   = glm::vec3(-3.f, 12.f, 0.f);
    _rightArm.bas      = glm::vec3( 0.f, 12.f, 0.f);
    _rightForeArm.bas  = glm::vec3( 3.f, 12.f, 0.f);
    _leftBigLeg.bas    = glm::vec3( 0.f,  7.f, 0.f);
    _leftSmallLeg.bas  = glm::vec3(-1.f,  4.f, 0.f);
    _rightBigLeg.bas   = glm::vec3( 0.f,  7.f, 0.f);
    _rightSmallLeg.bas = glm::vec3( 1.f,  4.f, 0.f);

	_head.end          = glm::vec3( 0.f, 12.f, 0.f);
    _body.end          = glm::vec3( 0.f,  7.f, 0.f);
    _leftArm.end       = glm::vec3(-3.f, 12.f, 0.f);
    _leftForeArm.end   = glm::vec3(-6.f, 12.f, 0.f);
    _rightArm.end      = glm::vec3( 3.f, 12.f, 0.f);
    _rightForeArm.end  = glm::vec3( 6.f, 12.f, 0.f);
    _leftBigLeg.end    = glm::vec3(-1.f,  4.f, 0.f);
    _leftSmallLeg.end  = glm::vec3(-2.f,  0.f, 0.f);
    _rightBigLeg.end   = glm::vec3( 1.f,  4.f, 0.f);
    _rightSmallLeg.end = glm::vec3( 2.f,  0.f, 0.f);

	_head.pos          = glm::vec3( 0.f,  0.f, 0.f);
    _body.pos          = glm::vec3( 0.f,  0.f, 0.f);
    _leftArm.pos       = glm::vec3( 0.f,  0.f, 0.f);
    _leftForeArm.pos   = glm::vec3( 0.f,  0.f, 0.f);
    _rightArm.pos      = glm::vec3( 0.f,  0.f, 0.f);
    _rightForeArm.pos  = glm::vec3( 0.f,  0.f, 0.f);
    _leftBigLeg.pos    = glm::vec3( 0.f,  0.f, 0.f);
    _leftSmallLeg.pos  = glm::vec3( 0.f,  0.f, 0.f);
    _rightBigLeg.pos   = glm::vec3( 0.f,  0.f, 0.f);
    _rightSmallLeg.pos = glm::vec3( 0.f,  0.f, 0.f);

	_head.rot          = glm::vec3( 0.f,  0.f, 0.f);
    _body.rot          = glm::vec3( 0.f,  0.f, 0.f);
    _leftArm.rot       = glm::vec3( 0.f,  0.f, 0.f);
    _leftForeArm.rot   = glm::vec3( 0.f,  0.f, 0.f);
    _rightArm.rot      = glm::vec3( 0.f,  0.f, 0.f);
    _rightForeArm.rot  = glm::vec3( 0.f,  0.f, 0.f);
    _leftBigLeg.rot    = glm::vec3( 0.f,  0.f, 0.f);
    _leftSmallLeg.rot  = glm::vec3( 0.f,  0.f, 0.f);
    _rightBigLeg.rot   = glm::vec3( 0.f,  0.f, 0.f);
    _rightSmallLeg.rot = glm::vec3( 0.f,  0.f, 0.f);

    // parts.emplace_back(_head);
    // parts.emplace_back(_body);
    // parts.emplace_back(_leftArm);
    // parts.emplace_back(_leftForeArm);
    // parts.emplace_back(_rightArm);
    // parts.emplace_back(_rightForeArm);
    // parts.emplace_back(_leftBigLeg);
    // parts.emplace_back(_leftSmallLeg);
    // parts.emplace_back(_rightBigLeg);
    // parts.emplace_back(_rightSmallLeg);

    topology.add(RigidBody("head", _head.bas, _head.end, _head.pos, _head.rot));
    topology.add("head", RigidBody("body", _body.bas, _body.end, _body.pos, _body.rot));
    topology.add("head", RigidBody("leftArm", _leftArm.bas, _leftArm.end, _leftArm.pos, _leftArm.rot));
    topology.add("leftArm", RigidBody("leftForeArm", _leftForeArm.bas, _leftForeArm.end, _leftForeArm.pos, _leftForeArm.rot));
    topology.add("head", RigidBody("rightArm", _rightArm.bas, _rightArm.end, _rightArm.pos, _rightArm.rot));
    topology.add("rightArm", RigidBody("rightForeArm", _rightForeArm.bas, _rightForeArm.end, _rightForeArm.pos, _rightForeArm.rot));
    topology.add("body", RigidBody("leftBigLeg", _leftBigLeg.bas, _leftBigLeg.end, _leftBigLeg.pos, _leftBigLeg.rot));
    topology.add("leftBigLeg", RigidBody("leftSmallLeg", _leftSmallLeg.bas, _leftSmallLeg.end, _leftSmallLeg.pos, _leftSmallLeg.rot));
    topology.add("body", RigidBody("rightBigLeg", _rightBigLeg.bas, _rightBigLeg.end, _rightBigLeg.pos, _rightBigLeg.rot));
    topology.add("rightBigLeg", RigidBody("rightSmallLeg", _rightSmallLeg.bas, _rightSmallLeg.end, _rightSmallLeg.pos, _rightSmallLeg.rot));

    parts.emplace_back(&_head);
    parts.emplace_back(&_body);
    parts.emplace_back(&_leftArm);
    parts.emplace_back(&_leftForeArm);
    parts.emplace_back(&_rightArm);
    parts.emplace_back(&_rightForeArm);
    parts.emplace_back(&_leftBigLeg);
    parts.emplace_back(&_leftSmallLeg);
    parts.emplace_back(&_rightBigLeg);
    parts.emplace_back(&_rightSmallLeg);
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.f, width_w / height_w, 0.1f, 100.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 7, 10, 0, 7, 0, 0, 1, 0);
}

void display()
{
    width_w  = (float) glutGet(GLUT_WINDOW_WIDTH);
    height_w = (float) glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, (int) width_w, (int) height_w);

    glClear(GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT
    glColor3f(1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.f, width_w / height_w, 0.1f, 100.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 7, 30, 0, 7, 0, 0, 1, 0);

    // topology.print();
    // exit(0);
    
    for (int i = 0; i < uids.size(); i++) {
        std::string uid = uids[i];
        triple_t& part = *(parts[i]);
        std::vector<Joint*> chain = topology.getLines(uid);
        // std::string os;
        // for (auto& s : chain) os += s->uid + " ";
        // std::cout << " " << uid << "|" << os << "\n";
        glm::mat4 transform(1.f);
        for (auto & parent : chain)
            transform = transform * parent->rigidBody.iState();
        glm::vec3 p_end = glm::vec3(transform * glm::vec4(part.end, 1.f));
        // part.end = p0;
        // part.end.x = p0.x; part.end.y = p0.y; part.end.z = p0.z;
        if (!chain.back()->children.empty()) {
            for (auto& child : chain.back()->children) {
                unsigned int idx = 0;
                while (idx < uids.size()) {
                    if (uids[idx] == child->uid) break;
                    idx++;
                }
                if (idx >= uids.size()) {
                    std::cout << "No joint (" << child << ")!";
                    exit(1);
                }
                parts[idx]->bas = p_end;
                // parts[idx]->bas.y = part.end.y;
                // parts[idx]->bas.z = part.end.z;
                // std::cout << "update child : " << child->uid << "; parent : " << chain.back()->uid << "\n ";
            }
        }
        // if (i == 9) {
		// 	std::cout.setf(std::ios::fixed);
        //     std::cout << ">> " << std::setprecision(4) << transform[0][0] << "," 
		// 		<< std::setprecision(4) << transform[0][1] << "," << std::setprecision(4) << transform[0][2] << "," << std::setprecision(4) << transform[0][3]
        //     << "," << std::setprecision(4) << transform[1][0] << "," << std::setprecision(4) << transform[1][1] << "," << std::setprecision(4)
		// 		<< transform[1][2] << "," << std::setprecision(4) << transform[1][3]
        //     << "," << std::setprecision(4) << transform[2][0] << "," << std::setprecision(4) << transform[2][1] << "," << std::setprecision(4)
		// 		<< transform[2][2] << "," << std::setprecision(4) << transform[2][3]
        //     << "," << std::setprecision(4) << transform[3][0] << "," << std::setprecision(4)
		// 		<< transform[3][1] << "," << std::setprecision(4) << transform[3][2] << "," << std::setprecision(4) << transform[3][3] << std::endl;
        // }
        // if (i == 9) {
			//printf(">>  %d | %.2f, %.2f, %.2f   %.2f, %.2f, %.2f\n", parents.size(), p0.x, p0.y, p0.z, p1.x, p1.y, p1.z);
            // std::cout << ">> " << std::setprecision(4) << p0.x << "," << std::setprecision(4) << p0.y << "," << std::setprecision(4) << p0.z
            // << "  " << std::setprecision(4) << p1.x << "," << std::setprecision(4) << p1.y << "," << std::setprecision(4) << p1.z
            // << std::endl;
        // }
        glColor3f(0.8, 0.8, 0.8);
        glLineWidth(5);
        glBegin(GL_LINES);
        glVertex3f(part.bas.x, part.bas.y, part.bas.z);
        glVertex3f(p_end.x, p_end.y, p_end.z);
        glEnd();
    }
    // exit(0);


    glUseProgram(0);
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    ImGui::Begin("Body");
    // ImGui::SliderFloat("head pos x", &(_head.pos.x), -10.0f, 10.0f);
    // ImGui::SliderFloat("head pos y", &(_head.pos.y), -10.0f, 10.0f);
    ImGui::SliderFloat("head rotate z", &(_head.rot.z), -1000.0f, 1000.0f);
    // ImGui::SliderFloat("body rotate z", &(std::get<2>(_body).z), -180.0f, 180.0f);
    ImGui::SliderFloat("leftArm rotate z", &(_leftArm.rot.z), -180.0f, 180.0f);
    ImGui::SliderFloat("leftForeArm rotate z", &(_leftForeArm.rot.z), -180.0f, 180.0f);
    ImGui::SliderFloat("rightArm rotate z", &(_rightArm.rot.z), -180.0f, 180.0f);
    ImGui::SliderFloat("rightForeArm rotate z", &(_rightForeArm.rot.z), -180.0f, 180.0f);
    ImGui::SliderFloat("leftBigLeg rotate z", &(_leftBigLeg.rot.z), -180.0f, 180.0f);
    ImGui::SliderFloat("leftSmallLeg rotate z", &(_leftSmallLeg.rot.z), -180.0f, 180.0f);
    ImGui::SliderFloat("rightBigLeg rotate z", &(_rightBigLeg.rot.z), -180.0f, 180.0f);
    ImGui::SliderFloat("rightSmallLeg rotate z", &(_rightSmallLeg.rot.z), -90.0f, 90.0f);

    ImGui::End();
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

//     TF& tf = *timer;
// #if _WIN32
// 	float iTime = tf.iTime() * 1000 * 90;
// #elif linux
// 	float iTime = tf.iTime() * 1000 * 1.5;
// #endif
//     body[0].position.x = (float) cos(M_PI / 180 * iTime);
//     body[0].position.y = (float) sin(M_PI / 180 * iTime);
//     body[0].eular.z = iTime;
//     body[1].eular.z = iTime;
//     std::cout << "iTime : " << iTime
//             << "\t (" << points[0].x << "," << points[0].y << "," << points[0].z << ") - (" << points[1].x << "," << points[1].y << "," << points[1].z
//             << ") ### (" << points[2].x << "," << points[2].y << "," << points[2].z << ") - (" << points[3].x << "," << points[3].y << "," << points[3].z << ")"
//             << std::endl;
    // glm::mat4 accumulate(1.f);
    // std::vector<glm::mat4> matrices;
    // for (unsigned int i = 0; i < coordinates.size(); i++) {
    //     accumulate = accumulate * body[i].iState();
    //     matrices.emplace_back(accumulate);

    //     glm::vec3 p0 = glm::vec3(matrices[i] * glm::vec4(points[2 * i], 1.f)),
    //     p1 = glm::vec3(matrices[i] * glm::vec4(points[2 * i + 1], 1.f));

    //     glColor3f(0.8, 0.8, 0.8);
    //     glLineWidth(2);
    //     glBegin(GL_LINES);
    //     glVertex3f(p0.x, p0.y, p0.z);
    //     glVertex3f(p1.x, p1.y, p1.z);
    //     glEnd();

    //     glPointSize(5);
    //     glBegin(GL_POINTS);
    //     glVertex3f(0, 0, 0);
    //     glEnd();
    // }

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

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();
}
MAIN_EN