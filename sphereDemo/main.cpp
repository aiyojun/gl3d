//#pragma once

//class sphere {
//public:
//
//};

#define MAIN_BE int main(int argc, char** argv) {
#define MAIN_EN return 0;}

#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <exception>

#define F_PI 3.1415926f

struct vertex_t { float x, y, z; unsigned int i; float s, t;};
struct mesh_t {std::vector<vertex_t> vertices; std::vector<unsigned int> indices;};

float keep(float x)
{ return (float) ((int) (x * 10000)) / 10000; }
vertex_t keep(vertex_t v)
{ return vertex_t {keep(v.x), keep(v.y), keep(v.z), v.i, keep(v.s), keep(v.t)}; }


mesh_t g_sphere(float r, int t, int w, int h) {
	// std::cout << "-- 1" << std::endl;
    float delta_w = (float) w / t, delta_h = (float) h / t, delta_angle = 2 * F_PI / t;
    int t_2 = (int) (t / 2);
    int t_4 = (int) (t / 4);
    std::vector<vertex_t> vertices;
    std::vector<std::vector<unsigned int>> vertices_indices;
	// std::cout << "-- 2" << std::endl;
    for (int i = 0; i < t_2 + 1; i++) vertices_indices.emplace_back(std::vector<unsigned int>());

    {
        int i = - t_4;
        float alpha = delta_angle * i;
        vertex_t _v {
            r * cos(alpha) * sin(0),
            r * sin(alpha),
            r * cos(alpha) * cos(0),
            vertices.size(),
            0.f,
            0.f
        };
        vertices_indices[t_4 - i].emplace_back(vertices.size());
        vertices.emplace_back(keep(_v));
    }

    for (int j = 0; j < t; ++j) {
        float beta = j * delta_angle;
        for (int i = - t_4; i <= t_4; i++) {
            int mul_f = t_4 - abs(i);
            float alpha = delta_angle * i;
            if (i == - t_4) { continue; } else if (i == t_4) { continue; }
            vertex_t v {
                r * cos(alpha) * sin(beta),
                r * sin(alpha),
                r * cos(alpha) * cos(beta),
                vertices.size(),
                delta_w * j,
                delta_h * (t_4 + i)
            };
            vertices_indices[t_4 - i].emplace_back(vertices.size());
            vertices.emplace_back(keep(v));
            if (mul_f > 1) {
                for (int k = 1; k < mul_f; k++) {
                    vertex_t v_ {
                        r * cos(alpha) * sin(beta + delta_angle / mul_f * k),
                        r * sin(alpha),
                        r * cos(alpha) * cos(beta + delta_angle / mul_f * k),
                        vertices.size(),
                        delta_w * j + delta_w / mul_f * k,
                        delta_h * (t_4 + i)
                    };
                    vertices_indices[t_4 - i].emplace_back(vertices.size());
                    vertices.emplace_back(keep(v_));
                }
            }
        }
    }

    {
        int i = t_4;
        float alpha = delta_angle * i;
        vertex_t _v {
            r * cos(alpha) * sin(0),
            r * sin(alpha),
            r * cos(alpha) * cos(0),
            vertices.size(),
            0.f,
            1.f
        };
        vertices_indices[t_4 - i].emplace_back(vertices.size());
        vertices.emplace_back(keep(_v));
    }
    // std::cout << ">> " << vertices.size() << std::endl;
	// std::cout << "-- 3" << std::endl;

    // for (int i = 0; i < vertices_indices.size(); i++) {
    //     std::string s;
    //     for (int j = 0; j < vertices_indices[i].size(); j++) {
    //         s += (std::to_string(vertices_indices[i][j]) + (j != vertices_indices[i].size() - 1 ? "," : ""));
    //     }
    //     std::cout << i << ". " << vertices_indices[i].size() << " | " << s << std::endl; 
    // }

    // draw triangles
    std::vector<unsigned int> triangles;
    for (int i = 0; i < t; i++) {
        triangles.emplace_back(vertices_indices[0][0]);
        triangles.emplace_back(vertices_indices[1][i]);
        if (i + 1 == vertices_indices[1].size()) {
            triangles.emplace_back(vertices_indices[1][0]);
        } else if (i + 1 > vertices_indices[1].size()) {
            throw std::runtime_error("error 5");
        } else {
            triangles.emplace_back(vertices_indices[1][i + 1]);
        }
    }
	// std::cout << "-- 4" << std::endl;
    for (int i = 0; i < t; i++) {
        triangles.emplace_back(vertices_indices[t_2 - 1][i]);
        triangles.emplace_back(vertices_indices[t_2][0]);
        if (i + 1 == vertices_indices[t_2 - 1].size()) {
            triangles.emplace_back(vertices_indices[t_2 - 1][0]);
        } else if (i + 1 > vertices_indices[t_2 - 1].size()) {
            throw std::runtime_error("error 6");
        } else {
            triangles.emplace_back(vertices_indices[t_2 - 1][i + 1]);
        }
    }
	
	// for (int i = 0; i < vertices_indices.size(); i++) {
	// 	std::cout << i << ". " << vertices_indices[i].size() << std::endl;
	// }
	// std::cout << "-- 5" << std::endl;
    for (int i = 1; i < t_4; i++) {
        // std::cout << "[i] " << i << std::endl;
        for (int j = 0; j < t; j++) {
            int mul_f = i;
            int mul_f_o = i + 1;
            for (int k = 0; k < mul_f; k++) {
                // std::cout << "-- index(+0): " << j * mul_f + k << std::endl;
                unsigned int p0 = vertices_indices[i][j * mul_f + k];
                unsigned int p1 = vertices_indices[i + 1][(j * mul_f_o + k + 1 >= vertices_indices[i + 1].size() ? 0 : j * mul_f_o + k + 1)];
                if (p0 > vertices.size()) { std::cout << "P0 out out range 3!" << p0 << std::endl; }
                if (p1 > vertices.size()) { std::cout << "P1 out out range 4!" << p1 << std::endl; }
                triangles.emplace_back(p0);
                triangles.emplace_back(p1);
                if (j * mul_f + k + 1 == vertices_indices[i].size()) {
                    triangles.emplace_back(vertices_indices[i][0]);
                } else if (j * mul_f + k + 1 > vertices_indices[i].size()) {
					std::cout << "error 7" << std::endl;
                    throw std::runtime_error("error 7");
                } else {
                    triangles.emplace_back(vertices_indices[i][j * mul_f + k + 1]);
                }
            }
            for (int k = 0; k < mul_f_o; k++) {
                // std::cout << "-- index(+1): " << j * mul_f_o + k << std::endl;
                unsigned int p0 = vertices_indices[i + 1][j * mul_f_o + k];
                if (p0 > vertices.size()) { std::cout << "P0 out out range 1!" << p0 << std::endl; }
                triangles.emplace_back(p0);
                if (j * mul_f_o + k + 1 == vertices_indices[i + 1].size()) {
                    triangles.emplace_back(vertices_indices[i + 1][0]);
                } else if (j * mul_f_o + k + 1 > vertices_indices[i + 1].size()) {
					std::cout << "error 8" << std::endl;
                    throw std::runtime_error("error 8");
                } else {
                    triangles.emplace_back(vertices_indices[i + 1][j * mul_f_o + k + 1]);
                }
                unsigned int p1 = vertices_indices[i][(j * mul_f + k >= vertices_indices[i].size() ? 0 : j * mul_f + k)];
                if (p1 > vertices.size()) { std::cout << "P1 out out range 2!" << p1 << std::endl; }
                //if ((j * mul_f_o + k) == 79) { std::cout << "-- j * mul_f + k : " << (j * mul_f + k) << std::endl;}
                triangles.emplace_back(p1);
            }
        }
        for (int j = 0; j < t; j++) {
            int mul_f = i;
            int mul_f_o = i + 1;
            for (int k = 0; k < mul_f; k++) {
                triangles.emplace_back(vertices_indices[t_2 - i - 1][(j * mul_f_o + k + 1 >= vertices_indices[t_2 - i - 1].size() ? 0 : j * mul_f_o + k + 1)]);
                triangles.emplace_back(vertices_indices[t_2 - i][(j * mul_f + k >= vertices_indices[t_2 - i].size() ? 0 : j * mul_f + k)]);
                if (j * mul_f + k + 1 == vertices_indices[t_2 - i].size()) {
                    triangles.emplace_back(vertices_indices[t_2 - i][0]);
                } else if (j * mul_f + k + 1 > vertices_indices[t_2 - i].size()) {
					std::cout << "error 9" << std::endl;
                    throw std::runtime_error("error 9");
                } else {
                    triangles.emplace_back(vertices_indices[t_2 - i][j * mul_f + k + 1]);
                }
            }
            for (int k = 0; k < mul_f_o; k++) {
                if (j * mul_f_o + k + 1 == vertices_indices[t_2 - i - 1].size()) {
                    triangles.emplace_back(vertices_indices[t_2 - i - 1][0]);
                } else if (j * mul_f_o + k + 1 > vertices_indices[t_2 - i - 1].size()) {
					std::cout << "error 10" << std::endl;
                    throw std::runtime_error("error 10");
                } else {
                    triangles.emplace_back(vertices_indices[t_2 - i - 1][j * mul_f_o + k + 1]);
                }
                triangles.emplace_back(vertices_indices[t_2 - i - 1][(j * mul_f_o + k >= vertices_indices[t_2 - i - 1].size() ? 0 : j * mul_f_o + k)]);
                triangles.emplace_back(vertices_indices[t_2 - i][(j * mul_f + k >= vertices_indices[t_2 - i].size() ? 0 : j * mul_f + k)]);
            }
        }
    }
	// std::cout << "---------------" << std::endl;
    // int count = 0;
    // while (count < triangles.size()) {
    //     std::cout << "[Triangle]" << triangles[count] << ", " << triangles[count + 1] << ", " << triangles[count + 2] << std::endl;
    //     count = count + 3;
    // }
    return {vertices, triangles};
}

MAIN_BE
{
    // std::cout << sin(F_PI / 4) << std::endl;exit(0);

    float t = 40, w = 1, h = 1, r = 4;
    std::cout << "# Blender v2.83.5 OBJ File: \n# www.blender.org\nmtllib sphere.mtl\no sphere.obj" << std::endl;

    mesh_t sphere = g_sphere(r, t, w, h);
    for (auto& e : sphere.vertices) {
        std::cout << "v " << e.x << " " << e.y << " " << e.z << std::endl;
    }
    for (auto& e : sphere.vertices) {
        std::cout << "vt " << e.s << " " << e.t << std::endl;
    }
    for (auto& e : sphere.vertices) {
        std::cout << "vn " << e.x << " " << e.y << " " << e.z << std::endl;
    }
    std::cout << "usemtl polar_white\ns off\n";
    for (int i = 0; i < sphere.indices.size();) {
        std::cout << "f " 
        << sphere.indices[i + 0] + 1 << "/" << sphere.indices[i + 0] + 1 << "/" << sphere.indices[i + 0] + 1 << " " 
        << sphere.indices[i + 1] + 1 << "/" << sphere.indices[i + 1] + 1 << "/" << sphere.indices[i + 1] + 1 << " " 
        << sphere.indices[i + 2] + 1 << "/" << sphere.indices[i + 2] + 1 << "/" << sphere.indices[i + 2] + 1 << std::endl;
        i += 3;
    }
//    return triangles;
}
MAIN_EN

