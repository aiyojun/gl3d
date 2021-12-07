#pragma once

#include <string>

class shader_t {
public:
    unsigned int shader_id;
    static std::string read_file(const std::string& path);
    static unsigned int create_shader(int shader_type, const char* source);
    static unsigned int create_program(unsigned int shaders[], int n);

    void init(const std::string& path);
    void use();
    void set_int(const std::string& name, int value);
    void set_bool(const std::string& name, bool value);
    void set_float(const std::string &name, float value);
    void set_vec2(const std::string &name, const glm::vec2 &value);
    void set_vec2(const std::string &name, float x, float y);
    void set_vec3(const std::string &name, const glm::vec3 &value);
    void set_vec3(const std::string &name, float x, float y, float z);
    void set_vec4(const std::string &name, const glm::vec4 &value);
    void set_vec4(const std::string &name, float x, float y, float z, float w);
    void set_mat2(const std::string &name, const glm::mat2 &mat);
    void set_mat3(const std::string &name, const glm::mat3 &mat);
    void set_mat4(const std::string &name, const glm::mat4 &mat);

    static void use(unsigned int shader);
    static void set_int(unsigned int shader, const std::string& name, int value);
    static void set_bool(unsigned int shader, const std::string& name, bool value);
    static void set_float(unsigned int shader, const std::string &name, float value);
    static void set_vec2(unsigned int shader, const std::string &name, const glm::vec2 &value);
    static void set_vec2(unsigned int shader, const std::string &name, float x, float y);
    static void set_vec3(unsigned int shader, const std::string &name, const glm::vec3 &value);
    static void set_vec3(unsigned int shader, const std::string &name, float x, float y, float z);
    static void set_vec4(unsigned int shader, const std::string &name, const glm::vec4 &value);
    static void set_vec4(unsigned int shader, const std::string &name, float x, float y, float z, float w);
    static void set_mat2(unsigned int shader, const std::string &name, const glm::mat2 &mat);
    static void set_mat3(unsigned int shader, const std::string &name, const glm::mat3 &mat);
    static void set_mat4(unsigned int shader, const std::string &name, const glm::mat4 &mat);
};

#if defined(ALL_IMPL)

#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

std::string shader_t::read_file(const std::string& path)
{
    std::string r;
    std::ifstream fp;
    fp.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        fp.open(path.c_str());
        std::stringstream pipeline;
        pipeline << fp.rdbuf();
        fp.close();
        r = pipeline.str();
    } catch (std::ifstream::failure &) {
        std::cout << std::string("Read [") + path + "] failed!\n";
        exit(1);
    }
    return std::move(r);
}

static std::vector<std::string> string_split(const std::string& s, const std::string& demi)
{
    std::string eat(s);
    std::vector<std::string> _r;
    while (!eat.empty()) {
        std::string::size_type index = eat.find(demi);
        if (index == std::string::npos) {
            _r.emplace_back(eat); eat = "";
            continue;
        }
        _r.emplace_back(eat.substr(0, index));
        eat = eat.substr(index + demi.length());
    }
    return _r;
}

void shader_t::init(const std::string& path)
{
    std::vector<std::string> shaders_source = string_split(read_file(path), "// --\n");
    if (shaders_source.size() < 2) {
        std::cout << "Shader at least vertex & fragment!" << std::endl;
        exit(1);
    }
    std::cout << "****************************************" << std::endl;
    std::cout << "*********** shader vertex **************" << std::endl;
    std::cout << "****************************************" << std::endl;
    std::cout << shaders_source[0] << std::endl;
    std::cout << "****************************************" << std::endl;
    std::cout << "*********** shader fragment ************" << std::endl;
    std::cout << "****************************************" << std::endl;
    std::cout << shaders_source[1] << std::endl;
    int state_code = 0;
    char err_msg[512];
    unsigned int id_v = create_shader(0, shaders_source[0].c_str());
    unsigned int id_f = create_shader(1, shaders_source[1].c_str());
    unsigned int shaders[2] = {id_v, id_f};
    shader_id = create_program(shaders, 2);
}

unsigned int shader_t::create_shader(int shader_type, const char* source)
{
    unsigned int r = glCreateShader(!shader_type ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
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

unsigned int shader_t::create_program(unsigned int shaders[], int n)
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

inline void shader_t::use()
{use(shader_id);}

inline void shader_t::set_int(const std::string& name, int value)
{set_int(shader_id, name, value);}

inline void shader_t::set_bool(const std::string& name, bool value)
{set_bool(shader_id, name, (int) value);}

inline void shader_t::set_float(const std::string &name, float value)
{set_float(shader_id, name, value);}

inline void shader_t::set_vec2(const std::string &name, const glm::vec2 &value)
{set_vec2(shader_id, name, value);}

inline void shader_t::set_vec2(const std::string &name, float x, float y)
{set_vec2(shader_id, name, x, y);}

inline void shader_t::set_vec3(const std::string &name, const glm::vec3 &value)
{set_vec3(shader_id, name, value);}

inline void shader_t::set_vec3(const std::string &name, float x, float y, float z)
{set_vec3(shader_id, name, x, y, z);}

inline void shader_t::set_vec4(const std::string &name, const glm::vec4 &value)
{set_vec4(shader_id, name, value);}

inline void shader_t::set_vec4(const std::string &name, float x, float y, float z, float w)
{set_vec4(shader_id, name, x, y, z, w);}

inline void shader_t::set_mat2(const std::string &name, const glm::mat2 &mat)
{set_mat2(shader_id, name, mat);}

inline void shader_t::set_mat3(const std::string &name, const glm::mat3 &mat)
{set_mat3(shader_id, name, mat);}

inline void shader_t::set_mat4(const std::string &name, const glm::mat4 &mat)
{set_mat4(shader_id, name, mat);}

void shader_t::use(unsigned int shader)
{glUseProgram(shader);}

void shader_t::set_int(unsigned int shader, const std::string& name, int value)
{glUniform1i(glGetUniformLocation(shader, name.c_str()), value);}

void shader_t::set_bool(unsigned int shader, const std::string& name, bool value)
{glUniform1i(glGetUniformLocation(shader, name.c_str()), (int) value);}

void shader_t::set_float(unsigned int shader, const std::string &name, float value)
{glUniform1f(glGetUniformLocation(shader, name.c_str()), value);}

void shader_t::set_vec2(unsigned int shader, const std::string &name, const glm::vec2 &value)
{glUniform2fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);}

void shader_t::set_vec2(unsigned int shader, const std::string &name, float x, float y)
{glUniform2f(glGetUniformLocation(shader, name.c_str()), x, y);}

void shader_t::set_vec3(unsigned int shader, const std::string &name, const glm::vec3 &value)
{glUniform3fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);}

void shader_t::set_vec3(unsigned int shader, const std::string &name, float x, float y, float z)
{glUniform3f(glGetUniformLocation(shader, name.c_str()), x, y, z);}

void shader_t::set_vec4(unsigned int shader, const std::string &name, const glm::vec4 &value)
{glUniform4fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);}

void shader_t::set_vec4(unsigned int shader, const std::string &name, float x, float y, float z, float w)
{glUniform4f(glGetUniformLocation(shader, name.c_str()), x, y, z, w);}

void shader_t::set_mat2(unsigned int shader, const std::string &name, const glm::mat2 &mat)
{glUniformMatrix2fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);}

void shader_t::set_mat3(unsigned int shader, const std::string &name, const glm::mat3 &mat)
{glUniformMatrix3fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);}

void shader_t::set_mat4(unsigned int shader, const std::string &name, const glm::mat4 &mat)
{glUniformMatrix4fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);}

#endif