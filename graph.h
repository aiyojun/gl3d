#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "basic_type.h"

namespace geo {
    /**
     * Cone generator
     * @param   o
     *          center point of bottom circle
     * @param   p
     *          top point
     * @param   radius
     *          radius of bottom circle
     * @param   color
     *          color of cone
     * @param   n
     *          separating number
     * @return  mesh
     */
    mesh_t GenCone(const glm::vec3& o, const glm::vec3& p,
                   float radius, const glm::vec3& color,
                   int n = 100);

    /**
     * cylinder generator
     * @param   o
     *          center point of bottom circle
     * @param   p
     *          center point of top circle
     * @param   radius
     *          radius of bottom and top circle
     * @param   color
     *          color of cylinder
     * @param   n
     *          separating number
     * @return  mesh
     */
    mesh_t GenCylinder(const glm::vec3& o, const glm::vec3& p,
                       float radius, const glm::vec3& color,
                       int n = 100);

    /**
     * Cylinder generator(cone-like)
     * @param   o
     *          center point of bottom circle
     * @param   p
     *          center point of top circle
     * @param   ro
     *          radius of bottom circle
     * @param   rp
     *          radius of top circle
     * @param   color
     *          color of cylinder
     * @param   n
     *          separating number
     * @return  mesh
     */
    mesh_t GenCylinder(const glm::vec3& o, const glm::vec3& p,
                       float ro, float rp, const glm::vec3& color,
                       int n = 100);

    /**
     * Cylinder(cone-like) space judgement
     * @param   q
     *          any point
     * @param   o
     *          center point of bottom circle
     * @param   p
     *          center point of top circle
     * @param   ro
     *          radius of bottom circle
     * @param   rp
     *          radius of top circle
     * @param   color
     *          color of cylinder
     * @return  whether point is in cylinder space
     */
    bool PointInCylinder(const glm::vec3& q,
                         const glm::vec3& o, const glm::vec3& p,
                         float ro, float rp);

    /**
     * Sphere generator
     * @param   radius
     *          radius of sphere
     * @param   n
     *          separating number
     * @param   w, h
     *          texture width and height
     * @return  mesh
     */
    mesh_t GenSphere(float radius, int n = 80, float w = 0, float h = 0);
}

#define ALL_IMPL

#if defined(ALL_IMPL)

#include "glm_helper.h"

bool PointInCylinder(const glm::vec3& q, const glm::vec3& o, const glm::vec3& p, float ro, float rp)
{
    glm::vec3 uv_op(glm::normalize(p - o));
    glm::vec3 uv_oq(glm::normalize(q - o));
    float u_cos = glm::dot(uv_op, uv_oq);
    float l     = glm::distance(o, q) * u_cos;
    float h     = glm::distance(o, q) * glm::distance(glm::vec3(0.f), glm::cross(uv_op, uv_oq));
    float pq    = glm::distance(glm::vec3(0.f), glm::vec3(q - p));
    float oq    = glm::distance(glm::vec3(0.f), uv_oq);
    if (u_cos >= 0) {
        return h <= (ro * pq + rp * oq) / glm::distance(glm::vec3(0.f), uv_op);
    } else {
        return false;
    }
}

inline mesh_t geo::GenCylinder(const glm::vec3 &o,
                               const glm::vec3 &p,
                               float radius,
                               const glm::vec3 &color,
                               int n)
{
    return geo::GenCylinder(o, p, radius, 0.f, color, n);
}


mesh_t geo::GenCone(const glm::vec3& o,
                    const glm::vec3& p,
                    float radius,
                    const glm::vec3& color,
                    int n)
{
    float h = 1;
    glm::vec2 tex(0.f);
    glm::vec3 oxz(0.f), top(0.f, h, 0.f), down_v(0.f, -1.f, 0.f);
    std::vector<vertex_t> vertices;
    std::vector<unsigned int> indices;
    ////////////////////////////////////////////////////////////////////////////
    float height = glm::distance(o, p);
    glm::mat4 transform = glm::scale(glm::mat4(1.f), glm::vec3(1.f, height, 1.f));
    glm::vec3 dir = p - o;
    transform = glm::rotate(transform, glm::radians(atan(dir.z / dir.y) * 180 / M_PI), glm::vec3(1.f, 0.f, 0.f));
    transform = glm::rotate(transform, glm::radians(atan(dir.x / dir.y) * 180 / M_PI), glm::vec3(0.f, 0.f, 1.f));
    transform = glm::translate(transform, o);
    ////////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < n; i++) {
        float alpha = M_PI * 2 / n * i;
        glm::vec3 edge_p(radius * cos(alpha), 0, radius * sin(alpha));
        glm::vec3 tangent(cos(alpha + M_PI / 2), 0.0f, sin(alpha + M_PI / 2));
        glm::vec3 norm = glm::normalize(glm::cross(tangent, top - edge_p));
        glm::vec3(transform * glm::vec4(top, 1.f));
        vertices.emplace_back({glm::transform_vertex(top), glm::transform_normal(norm), color, tex});
        vertices.emplace_back({glm::transform_vertex(edge_p), glm::transform_normal(norm), color, tex});
        vertices.emplace_back({glm::transform_vertex(edge_p), glm::transform_normal(down_v), color, tex});
        indices.emplace_back(3 * i);
        indices.emplace_back(3 * i + 1);
        indices.emplace_back(3 * ((i + 1) % n) + 1);
        indices.emplace_back(3 * n);
        indices.emplace_back(3 * ((i + 1) % n) + 2);
        indices.emplace_back(3 * i + 2);
    }
    vertices.emplace_back({glm::transform_vertex(oxz), glm::transform_normal(down_v), color, tex});
    return {vertices, indices};
}

Mesh geo::GenCylinder(const glm::vec3& o, const glm::vec3& p, float ro, float rp, const glm::vec3& color, int n)
{
    float h = 1;
    glm::vec2 tex(0.f);
    glm::vec3 oxz(0.f), oxz_h(0.f, h, 0.f),
    down_v(0.f, -1.f, 0.f), up_v(0.f, 1.f, 0.f);
    std::vector<vertex_t> vertices;
    std::vector<unsigned int> indices;
    ////////////////////////////////////////////////////////////////////////////
    float height = glm::distance(o, p);
    glm::mat4 transform = glm::scale(glm::mat4(1.f), glm::vec3(1.f, height, 1.f));
    glm::vec3 dir = p - o;
    transform = glm::rotate(transform, glm::radians(atan(dir.z / dir.y) * 180 / M_PI), glm::vec3(1.f, 0.f, 0.f));
    transform = glm::rotate(transform, glm::radians(atan(dir.x / dir.y) * 180 / M_PI), glm::vec3(0.f, 0.f, 1.f));
    transform = glm::translate(transform, o);
    ////////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < n; i++) {
        float alpha = M_PI * 2 / n * i;
        glm::vec3 edge_h(rp * cos(alpha), h, rp * sin(alpha));
        glm::vec3 edge_p(ro * cos(alpha), 0, ro * sin(alpha));
        glm::vec3 tangent(cos(alpha + M_PI / 2), 0.0f, sin(alpha + M_PI / 2));
        glm::vec3 norm = glm::normalize(glm::cross(tangent, edge_h - edge_p));
        glm::vec3(transform * glm::vec4(top, 1.f));
        vertices.emplace_back({glm::transform_vertex(edge_h), glm::transform_normal(norm), color, tex});
        vertices.emplace_back({glm::transform_vertex(edge_p), glm::transform_normal(norm), color, tex});
        vertices.emplace_back({glm::transform_vertex(edge_p), glm::transform_normal(down_v), color, tex});
        vertices.emplace_back({glm::transform_vertex(edge_h), glm::transform_normal(up_v), color, tex});
        indices.emplace_back(4 * n);
        indices.emplace_back(4 * i + 3);
        indices.emplace_back(4 * ((i + 1) % n) + 3);
        indices.emplace_back(4 * i);
        indices.emplace_back(4 * i + 1);
        indices.emplace_back(4 * ((i + 1) % n) + 1);
        indices.emplace_back(4 * i);
        indices.emplace_back(4 * ((i + 1) % n) + 1);
        indices.emplace_back(4 * ((i + 1) % n));
        indices.emplace_back(4 * n + 1);
        indices.emplace_back(4 * ((i + 1) % n) + 2);
        indices.emplace_back(4 * i + 2);
    }
    vertices.emplace_back({glm::transform_vertex(oxz_h), glm::transform_normal(up_v), color, tex});
    vertices.emplace_back({glm::transform_vertex(oxz), glm::transform_normal(down_v), color, tex});
    return {vertices, indices};
}

mesh_t geo::GenSphere(float r, int t, float w, float h)
{
    int t_2 = (int) (t / 2);
    int t_4 = (int) (t / 4);
    float delta_w = w / t, delta_h = h / t_2, delta_angle = 2 * F_PI / t;
    std::vector<vertex_t> vertices;
    std::vector<std::vector<unsigned int>> vertices_indices;
    vertices_indices.reserve(t_2 + 1);
    for (int i = 0; i < t_2 + 1; i++)
        vertices_indices.emplace_back(std::vector<unsigned int>());
    {/** bottom point */
        int i = - t_4;
        float alpha = delta_angle * i;
        vertex_t _v {
            (float) (r * std::cos(alpha) * sin(0)),
            (float) (r * std::sin(alpha)),
            (float) (r * std::cos(alpha) * cos(0)),
            static_cast<unsigned int>(vertices.size()),
            0.f,
            1.f - 0.f
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
                static_cast<float>(r * std::cos(alpha) * std::sin(beta)),
                static_cast<float>(r * std::sin(alpha)),
                static_cast<float>(r * std::cos(alpha) * std::cos(beta)),
                static_cast<unsigned int>(vertices.size()),
                delta_w * j,
                1.f - delta_h * (t_4 + i)
            };
            vertices_indices[t_4 - i].emplace_back(vertices.size());
            vertices.emplace_back(keep(v));
            if (mul_f > 1) {
                for (int k = 1; k < mul_f; k++) {
                    vertex_t v_ {
                        static_cast<float>(r * std::cos(alpha) * std::sin(beta + delta_angle / mul_f * k)),
                        static_cast<float>(r * std::sin(alpha)),
                        static_cast<float>(r * std::cos(alpha) * std::cos(beta + delta_angle / mul_f * k)),
                        static_cast<unsigned int>(vertices.size()),
                        delta_w * j + delta_w / mul_f * k,
                        1.f - delta_h * (t_4 + i)
                    };
                    vertices_indices[t_4 - i].emplace_back(vertices.size());
                    vertices.emplace_back(keep(v_));
                }
            }
        }
    }
    {/** top point */
        int i = t_4;
        float alpha = delta_angle * i;
        vertex_t _v {
            static_cast<float>(r * cos(alpha) * sin(0)),
            static_cast<float>(r * sin(alpha)),
            static_cast<float>(r * cos(alpha) * cos(0)),
            static_cast<unsigned int>(vertices.size()),
            0.f,
            1.f - 1.f
        };
        vertices_indices[t_4 - i].emplace_back(vertices.size());
        vertices.emplace_back(keep(_v));
    }
    /** draw triangles */
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
    for (int i = 1; i < t_4; i++) {
        for (int j = 0; j < t; j++) {
            int mul_f = i;
            int mul_f_o = i + 1;
            for (int k = 0; k < mul_f; k++) {
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
    return {vertices, triangles};
}

#endif