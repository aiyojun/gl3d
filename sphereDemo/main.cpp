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

mesh_t g_sphere(int r, int t, int w, int h) {
	std::cout << "-- 1" << std::endl;
    float delta_w = w / t, delta_h = h / t, delta_angle = 2 * F_PI / t;
    int t_2 = (int) (t / 2);
    int t_4 = (int) (t / 4);
    std::vector<vertex_t> vertices;
    std::vector<std::vector<unsigned int>> vertices_indices;
	std::cout << "-- 2" << std::endl;
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
        vertices.emplace_back(_v);
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
        vertices.emplace_back(_v);
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
                delta_h * i
            };
            vertices_indices[t_4 - i].emplace_back(vertices.size());
            vertices.emplace_back(v);
            if (mul_f > 1) {
                for (int k = 1; k < mul_f; k++) {
                    vertex_t v_ {
                        r * cos(alpha) * sin(beta + delta_angle / mul_f * k),
                        r * sin(alpha),
                        r * cos(alpha) * cos(beta + delta_angle / mul_f * k),
                        vertices.size(),
                        delta_w * j + delta_w / mul_f * k,
                        delta_h * i
                    };
                    vertices_indices[t_4 - i].emplace_back(vertices.size());
                    vertices.emplace_back(v_);
                }
            }
        }
    }
	std::cout << "-- 3" << std::endl;
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
	std::cout << "-- 4" << std::endl;
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
	
	for (int i = 0; i < vertices_indices.size(); i++) {
		std::cout << i << ". " << vertices_indices[i].size() << std::endl;
	}
	std::cout << "-- 5" << std::endl;
    for (int i = 1; i < t_4; i++) {
        for (int j = 0; j < t; j++) {
            int mul_f = i;
            int mul_f_o = i + 1;
            for (int k = 0; k < mul_f; k++) {
                triangles.emplace_back(vertices_indices[i][j * mul_f + k]);
                triangles.emplace_back(vertices_indices[i + 1][j * mul_f_o + k + 1]);
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
                triangles.emplace_back(vertices_indices[i + 1][j * mul_f_o + k]);
                if (j * mul_f_o + k + 1 == vertices_indices[i + 1].size()) {
                    triangles.emplace_back(vertices_indices[i + 1][0]);
                } else if (j * mul_f_o + k + 1 > vertices_indices[i + 1].size()) {
					std::cout << "error 8" << std::endl;
                    throw std::runtime_error("error 8");
                } else {
                    triangles.emplace_back(vertices_indices[i + 1][j * mul_f_o + k + 1]);
                }
                triangles.emplace_back(vertices_indices[i][j * mul_f + k]);
            }
        }
        for (int j = 0; j < t; j++) {
            int mul_f = i;
            int mul_f_o = i + 1;
            for (int k = 0; k < mul_f; k++) {
                triangles.emplace_back(vertices_indices[t_2 - i][j * mul_f + k]);
                triangles.emplace_back(vertices_indices[t_2 - i - 1][j * mul_f_o + k + 1]);
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
                triangles.emplace_back(vertices_indices[t_2 - i - 1][j * mul_f_o + k]);
                if (j * mul_f_o + k + 1 == vertices_indices[t_2 - i - 1].size()) {
                    triangles.emplace_back(vertices_indices[t_2 - i - 1][0]);
                } else if (j * mul_f_o + k + 1 > vertices_indices[t_2 - i - 1].size()) {
					std::cout << "error 10" << std::endl;
                    throw std::runtime_error("error 10");
                } else {
                    triangles.emplace_back(vertices_indices[t_2 - i - 1][j * mul_f_o + k + 1]);
                }
                triangles.emplace_back(vertices_indices[t_2 - i][j * mul_f + k]);
            }
        }
    }
	std::cout << "-- 6" << std::endl;
    return {vertices, triangles};
}

MAIN_BE
{
    float t = 100, w = 100, h = 48, r = 10;

    mesh_t sphere = g_sphere(r, t, w, h);
    for (auto& e : sphere.vertices) {
        std::cout << "v " << e.x << " " << e.y << " " << e.z << std::endl;
    }
    for (auto& e : sphere.vertices) {
        std::cout << "v " << e.s << " " << e.t << " " << e.z << std::endl;
    }
    for (auto& e : sphere.vertices) {
        std::cout << "vn " << -e.x << " " << -e.y << " " << -e.z << std::endl;
    }
    for (int i = 0; i < sphere.indices.size();) {
        std::cout << "f " 
        << sphere.indices[i + 0] << "/" << sphere.indices[i + 0] << "/" << sphere.indices[i + 0] << " " 
        << sphere.indices[i + 1] << "/" << sphere.indices[i + 1] << "/" << sphere.indices[i + 1] << " " 
        << sphere.indices[i + 2] << "/" << sphere.indices[i + 2] << "/" << sphere.indices[i + 2] << std::endl;
        i += 3;
    }
//    return triangles;
}
MAIN_EN

