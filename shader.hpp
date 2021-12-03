#pragma once

#include <string>
#include <glm/glm.hpp>

class shader_t {
public:
    unsigned int ID;
    void init(const char *vert, const char* frag);
    void use();
    void setInt(const std::string& name, int value);
    void setBool(const std::string& name, bool value);
    void setFloat(const std::string &name, float value);
    void setVec2(const std::string &name, const glm::vec2 &value);
    void setVec2(const std::string &name, float x, float y);
    void setVec3(const std::string &name, const glm::vec3 &value);
    void setVec3(const std::string &name, float x, float y, float z);
    void setVec4(const std::string &name, const glm::vec4 &value);
    void setVec4(const std::string &name, float x, float y, float z, float w);
    void setMat2(const std::string &name, const glm::mat2 &mat);
    void setMat3(const std::string &name, const glm::mat3 &mat);
    void setMat4(const std::string &name, const glm::mat4 &mat);
};

#if defined(SHADER_IMPL) || defined(ALL_IMPL)

#include <sstream>
#include <fstream>
#include <iostream>

static std::string read_file(const char *path)
{
    std::string r;
    std::ifstream fp;
    fp.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        fp.open(path);
        std::stringstream pipeline;
        pipeline << fp.rdbuf();
        fp.close();
        r = pipeline.str();
    } catch (std::ifstream::failure &) {
        std::cout << std::string("Read [") + path + "] failed!\n";
        throw std::string("Read [") + path + "] failed!";
    }
    return std::move(r);
}

static unsigned int create_shader(int shader_type, const char* source)
{
    unsigned int r = 0;
    if (!shader_type) {
        r = glCreateShader(GL_VERTEX_SHADER);
    } else {
        r = glCreateShader(GL_FRAGMENT_SHADER);
    }
    glShaderSource(r, 1, &source, nullptr);
    glCompileShader(r);
    int state_code = 0; char err_msg[512];
    glGetShaderiv(r, GL_COMPILE_STATUS, &state_code);
    if (!state_code) {
        glGetShaderInfoLog(r, 512, nullptr, err_msg);
        std::cout << (shader_type == 0 ? "[VERTEX] " : "[FRAGMENT] ")
                  << "Compilation failed: "
                  << err_msg << std::endl;
    }
    return r;
}

static unsigned int create_program(unsigned int shaders[], int n)
{
    unsigned int r = glCreateProgram();
    for (int i = 0; i < n; i++)
        glAttachShader(r, shaders[i]);
    glLinkProgram(r);
    int state_code = 0; char err_msg[512];
    glGetProgramiv(r, GL_LINK_STATUS, &state_code);
    if (!state_code) {
        glGetProgramInfoLog(r, 512, nullptr, err_msg);
        std::cout << "[PROGRAM] Link failed: "
                  << ": " << err_msg << std::endl;
    }
    for (int i = 0; i < n; i++)
        glDeleteShader(shaders[i]);
    return r;
}

void shader_t::init(const char *vert, const char* frag)
{
    std::string glsl_v = read_file(vert);
    std::string glsl_f = read_file(frag);
    std::cout << "****************************************" << std::endl;
    std::cout << "*********** shader vertex **************" << std::endl;
    std::cout << "****************************************" << std::endl;
    std::cout << glsl_v << std::endl;
    std::cout << "****************************************" << std::endl;
    std::cout << "*********** shader fragment ************" << std::endl;
    std::cout << "****************************************" << std::endl;
    std::cout << glsl_f << std::endl;
    int state_code = 0;
    char err_msg[512];
    unsigned int id_v = create_shader(0, glsl_v.c_str());
    unsigned int id_f = create_shader(1, glsl_f.c_str());
    unsigned int shaders[2] = {id_v, id_f};
    ID = create_program(shaders, 2);
}

inline void shader_t::use()
{glUseProgram(ID);}

inline void shader_t::setInt(const std::string& name, int value)
{glUniform1i(glGetUniformLocation(ID, name.c_str()), value);}

inline void shader_t::setBool(const std::string& name, bool value)
{glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);}

inline void shader_t::setFloat(const std::string &name, float value)
{glUniform1f(glGetUniformLocation(ID, name.c_str()), value);}

inline void shader_t::setVec2(const std::string &name, const glm::vec2 &value)
{glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);}

inline void shader_t::setVec2(const std::string &name, float x, float y)
{glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);}

inline void shader_t::setVec3(const std::string &name, const glm::vec3 &value)
{glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);}

inline void shader_t::setVec3(const std::string &name, float x, float y, float z)
{glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);}

inline void shader_t::setVec4(const std::string &name, const glm::vec4 &value)
{glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);}

inline void shader_t::setVec4(const std::string &name, float x, float y, float z, float w)
{glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);}

inline void shader_t::setMat2(const std::string &name, const glm::mat2 &mat)
{glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);}

inline void shader_t::setMat3(const std::string &name, const glm::mat3 &mat)
{glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);}

inline void shader_t::setMat4(const std::string &name, const glm::mat4 &mat)
{glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);}

#endif