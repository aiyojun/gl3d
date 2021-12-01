#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>

class j_mesh {
protected:
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
};

class j_point : public j_mesh {
public:
    void init(GLfloat size, const glm::vec3& p0, const glm::vec3& rgb) {
        this->size = size;
        // generate
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // bind data
        GLfloat vertices[] = {
                p0.x, p0.y, p0.z, 1.0f, 1.0f, 1.0f, rgb.r, rgb.g, rgb.b
        };
        glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);
        // interpret
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) sizeof(glm::vec3));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) (2 * sizeof(glm::vec3)));
        // unbind
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    void draw() {
        glBindVertexArray(vao);
        glPointSize(size);
        glDrawArrays(GL_LINES, 0, 1);
        glBindVertexArray(0);
    }
private:
    GLfloat size;
};

class j_line : public j_mesh {
public:
    void init(GLfloat width, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& rgb) {
        this->width = width;
        // generate
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // bind data
        GLfloat vertices[] = {
                p0.x, p0.y, p0.z, 1.0f, 1.0f, 1.0f, rgb.r, rgb.g, rgb.b,
                p1.x, p1.y, p1.z, 1.0f, 1.0f, 1.0f, rgb.r, rgb.g, rgb.b
        };
        glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);
        // interpret
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) sizeof(glm::vec3));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) (2 * sizeof(glm::vec3)));
        // unbind
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    void draw() {
        glBindVertexArray(vao);
        glLineWidth(width);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
    }
private:
    GLfloat width;
};

class j_box : public j_mesh {
public:
    void init(const glm::vec3& p0, float lon, float width, float height, const glm::vec3& rgb) {
        this->width = width;
        // generate
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // bind data
        GLfloat vertices[] = {
                p0.x, p0.y, p0.z, 1.0f, 1.0f, 1.0f, rgb.r, rgb.g, rgb.b,
//                p1.x, p1.y, p1.z, 1.0f, 1.0f, 1.0f, rgb.r, rgb.g, rgb.b
        };
        glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);
        // interpret
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) sizeof(glm::vec3));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) (2 * sizeof(glm::vec3)));
        // unbind
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    void draw() {
        glBindVertexArray(vao);
        glLineWidth(width);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
    }
private:
    GLfloat width;
};

class j_cone : public j_mesh {
public:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    void init(const glm::vec3& center, float radius, float height, const glm::vec3& color, int division=100) {
        vertices.clear(); indices.clear();
        glm::vec3 top_p = glm::vec3(center.x, center.y + height, center.z);
        for (int i = 0; i < division; i++) {
            float alpha = M_PI * 2 / division * i;
            glm::vec3 p = glm::vec3(center.x + radius * cos(alpha), center.y, center.z + radius * sin(alpha));
            glm::vec3 v1(cos(alpha + M_PI / 2), 0.0f, sin(alpha + M_PI / 2));
            glm::vec3 v2 = top_p - p;
            glm::vec3 norm = glm::normalize(glm::cross(v1, v2));
            vertices.emplace_back(p);
            vertices.emplace_back(norm);
            vertices.emplace_back(color);

            vertices.emplace_back(top_p);
            vertices.emplace_back(glm::vec3(1.0f));
            vertices.emplace_back(color);
        }
        for (int i = 0; i < division; i++) {
            if (i != division - 1) {
                vertices[2 * i * 3 + 4] = vertices[2 * i * 3 + 1] + vertices[2 * (i + 1) * 3 + 1];
            } else {
                vertices[2 * i * 3 + 4] = vertices[2 * i * 3 + 1] + vertices[2 * 0 * 3 + 1];
            }
        }
//        vertices.emplace_back(top_p);
//        vertices.emplace_back(glm::vec3(1.0f));
//        vertices.emplace_back(color);
        for (int i = 0; i < division; i++) {
            if (i != division - 1) {
                indices.emplace_back(2 * i);
                indices.emplace_back(2 * i + 1);
                indices.emplace_back(2 * i + 2);
//                indices.emplace_back(vertices.size() / 3 - 1);
//                indices.emplace_back(i);
//                indices.emplace_back(i + 1);
            } else {
                indices.emplace_back(2 * i);
                indices.emplace_back(2 * i + 1);
                indices.emplace_back(0);
//                indices.emplace_back(vertices.size() - 1);
//                indices.emplace_back(i);
//                indices.emplace_back(0);
            }
        }

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * 9 * sizeof(float), &(vertices[0]), GL_STATIC_DRAW);
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &(indices[0]), GL_STATIC_DRAW);
        // interpret
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) sizeof(glm::vec3));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) (2 * sizeof(glm::vec3)));

        // unbind
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    void draw() {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

class j_circle_rect : public j_mesh {
public:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    void init(const glm::vec3& center, float radius, float height, const glm::vec3& color, int division=100) {
        vertices.clear(); indices.clear();
        for (int i = 0; i < division; i++) {
            float alpha = M_PI * 2 / division * i;
            glm::vec3 p = glm::vec3(center.x + radius * cos(alpha), 0.0f, center.y + radius * sin(alpha));
            glm::vec3 t = glm::vec3(center.x + radius * cos(alpha), center.y + height, center.y + radius * sin(alpha));
            glm::vec3 norm = glm::normalize(p);
            vertices.emplace_back(p);
            vertices.emplace_back(norm);
            vertices.emplace_back(color);

            vertices.emplace_back(t);
            vertices.emplace_back(norm);
            vertices.emplace_back(color);
        }
        for (int i = 0; i < division; i++) {
            if (i != division - 1) {
                indices.emplace_back(2 * i);
                indices.emplace_back(2 * (i + 1));
                indices.emplace_back(2 * i + 1);

                indices.emplace_back(2 * i + 1);
                indices.emplace_back(2 * (i + 1));
                indices.emplace_back(2 * (i + 1) + 1);
            } else {
                indices.emplace_back(2 * i);
                indices.emplace_back(0);
                indices.emplace_back(2 * i + 1);

                indices.emplace_back(2 * i + 1);
                indices.emplace_back(0);
                indices.emplace_back(1);
            }
        }

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * 9 * sizeof(float), &(vertices[0]), GL_STATIC_DRAW);
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &(indices[0]), GL_STATIC_DRAW);
        // interpret
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) sizeof(glm::vec3));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) (2 * sizeof(glm::vec3)));

        // unbind
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    void draw() {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

class j_circle : public j_mesh {
public:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    void init(const glm::vec3& center, float radius, const glm::vec3& color, bool norm_up=true, int division=100) {
        glm::vec3 norm = glm::vec3(0.0f, 1.0f, 0.0f);
        if (!norm_up) {
            norm = glm::vec3(0.0f, -1.0f, 0.0f);
        }
        vertices.clear(); indices.clear();
        for (int i = 0; i < division; i++) {
            float alpha = M_PI * 2 / division * i;
            vertices.emplace_back(glm::vec3(center.x + radius * cos(alpha), center.y, center.z + radius * sin(alpha)));
            vertices.emplace_back(norm);
            vertices.emplace_back(color);
        }
        vertices.emplace_back(center);
        vertices.emplace_back(norm);
        vertices.emplace_back(color);
        for (int i = 0; i < division; i++) {
            if (i != division - 1) {
                indices.emplace_back(vertices.size() / 3 - 1);
                if (norm_up) {
                    indices.emplace_back(i);
                    indices.emplace_back(i + 1);
                } else {
                    indices.emplace_back(i + 1);
                    indices.emplace_back(i);
                }
            } else {
                indices.emplace_back(vertices.size() - 1);
                if (norm_up) {
                    indices.emplace_back(i);
                    indices.emplace_back(0);
                } else {
                    indices.emplace_back(0);
                    indices.emplace_back(i);
                }
            }
        }

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * 9 * sizeof(float), &(vertices[0]), GL_STATIC_DRAW);
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &(indices[0]), GL_STATIC_DRAW);
        // interpret
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) sizeof(glm::vec3));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) (2 * sizeof(glm::vec3)));

        // unbind
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    void draw() {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

class j_cylinder_cone {
public:
    void init(const glm::vec3& center, float radius, float height, const glm::vec3& color, int division=100) {
        _cone.init(center, radius, height, color, division);
        _circle.init(center, radius, color, false, division);
    }
    void draw() {
        _cone.draw();
        _circle.draw();
    }
private:
    j_cone _cone;
    j_circle _circle;
};

class j_cylinder {
public:
    j_circle_rect _circle_rect;
    j_circle _circle_bottom;
    j_circle _circle_top;
    void init(const glm::vec3& center, float radius, float height, const glm::vec3& color, int division=100) {
        _circle_bottom.init(center, radius, color, false, division);
        _circle_top.init(glm::vec3(center.x, center.y + height, center.z), radius, color, true, division);
        _circle_rect.init(center, radius, height, color, division);
    }
    void draw() {
        _circle_bottom.draw();
        _circle_top.draw();
        _circle_rect.draw();
    }
};

class j_face : public j_mesh {
public:
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;
    void init() {
        glm::vec3 rgb(1.0f, 1.0f, 1.0f);
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // bind data
        vertices.clear();
        indices.clear();
        vertices = {
                0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, rgb.r, rgb.g, rgb.b,
                2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, rgb.r, rgb.g, rgb.b,
                1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f, rgb.r, rgb.g, rgb.b,
                0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, rgb.r, rgb.g, rgb.b,
                -1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f, rgb.r, rgb.g, rgb.b,
                -2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, rgb.r, rgb.g, rgb.b,
                };
        indices.emplace_back(0);
        indices.emplace_back(1);
        indices.emplace_back(2);
        indices.emplace_back(3);
        indices.emplace_back(4);
        indices.emplace_back(5);
//        indices.emplace_back(0);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * 9 * sizeof(GLfloat), &(vertices[0]), GL_STATIC_DRAW);

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &(indices[0]), GL_STATIC_DRAW);

        // interpret
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) sizeof(glm::vec3));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) (2 * sizeof(glm::vec3)));
        // unbind
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void draw() {
        glBindVertexArray(vao);
        glDrawElements(GL_POLYGON, indices.size(), GL_UNSIGNED_INT, 0);
//        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
    }
};
