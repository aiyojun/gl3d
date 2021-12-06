#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

struct vertex_t {
    glm::vec3 position;
    glm::vec3 color;
};

#define beauty_green 0.47f, 0.64f, 0.33f
#define beauty_red   0.84f, 0.07f, 0.27f
#define dim_gray     0.54f, 0.55f, 0.56f

class coords_t {
public:
    float line_width = 1.8;
    void init();
    void render();
private:
    unsigned int vao = 0, vbo = 0, ebo = 0;
    std::vector<vertex_t> vertices;
    std::vector<unsigned int> indices;
};

#if defined(ALL_IMPL)

#include <GL/glew.h>

void coords_t::init()
{
    int idx = 0;
    // for (int i = 0; i < 1; i++) {
    for (int i = -100; i < 101; i++) {
        vertex_t vertex0, vertex1, vertex2, vertex3;
        vertex0.position = glm::vec3((float) i, 0, -100.f);
        vertex0.color    = i != 0 ? glm::vec3(dim_gray) : glm::vec3(beauty_red);
        vertex1.position = glm::vec3((float) i, 0,  100.f);
        vertex1.color    = i != 0 ? glm::vec3(dim_gray) : glm::vec3(beauty_red);
        vertex2.position = glm::vec3(-100.f, 0, (float) i);
        vertex2.color    = i != 0 ? glm::vec3(dim_gray) : glm::vec3(beauty_green);
        vertex3.position = glm::vec3( 100.f, 0, (float) i);
        vertex3.color    = i != 0 ? glm::vec3(dim_gray) : glm::vec3(beauty_green);
        vertices.emplace_back(vertex0);
        vertices.emplace_back(vertex1);
        vertices.emplace_back(vertex2);
        vertices.emplace_back(vertex3);
        indices.emplace_back(idx * 4);
        indices.emplace_back(idx * 4 + 1);
        indices.emplace_back(idx * 4 + 2);
        indices.emplace_back(idx * 4 + 3);
        idx++;
    }
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex_t), &(vertices[0]), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &(indices[0]), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void coords_t::render()
{
    glBindVertexArray(vao);
    glLineWidth(line_width);
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

#endif