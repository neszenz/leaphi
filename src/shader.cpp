#include "shader.hpp"
#include <iostream>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include <assert.h>
#include <stdexcept>
#include <fstream>
#include <sstream>

#include "util.hpp"

void read_source(const char* path, std::string& source) {
    assert(source.size() == 0);

    std::ifstream file(path);
    if (!file.is_open()) {
        std::stringstream ss;
        ss << "read_source() error for path \'" << path << "\'";
        throw std::runtime_error(ss.str());
    }

    std::stringstream ss;
    ss << file.rdbuf();
    source = ss.str();
}

unsigned compile_shader(const char* source, unsigned type) {
    unsigned shader_id;

    GL(shader_id = glCreateShader(type));
    GL(glShaderSource(shader_id, 1, &source, NULL));
    GL(glCompileShader(shader_id));

    int success;
    char info_log[512];
    GL(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success));
    if (!success) {
        GL(glGetShaderInfoLog(shader_id, 512, NULL, info_log));
        std::string st = (type == GL_VERTEX_SHADER)? "vertex" : "fragment";
        std::stringstream ss;
        ss << st << " shader compiling error: \n" << info_log;
        throw std::runtime_error(ss.str());
    }

    return shader_id;
}

// public interface  +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

Shader::Shader(const char* name, const char* v_path, const char* f_path) : m_program_name(name) {
    std::string v_source;
    std::string f_source;

    read_source(f_path, f_source);
    read_source(v_path, v_source);

    unsigned v_shader_id = compile_shader(v_source.c_str(), GL_VERTEX_SHADER);
    unsigned f_shader_id = compile_shader(f_source.c_str(), GL_FRAGMENT_SHADER);

    GL(m_program_id = glCreateProgram());
    GL(glAttachShader(m_program_id, v_shader_id));
    GL(glAttachShader(m_program_id, f_shader_id));
    GL(glLinkProgram(m_program_id));

    int success;
    char info_log[512];
    GL(glGetProgramiv(m_program_id, GL_LINK_STATUS, &success));
    if (!success) {
        GL(glGetProgramInfoLog(m_program_id, 512, NULL, info_log));
        std::stringstream ss;
        ss << "\'"<< m_program_name << "\' shader linking error: \n" << info_log;
        throw std::runtime_error(ss.str());
    }

    GL(glDeleteShader(v_shader_id));
    GL(glDeleteShader(f_shader_id));
}

Shader::~Shader() {
    GL(glUseProgram(0));
    GL(glDeleteProgram(m_program_id));
}

void Shader::use() const {
    GL(glUseProgram(m_program_id));
}

void Shader::uniform(const std::string& name, float value) {
    this->use();
    unsigned location = this->uniform_location(name);
    GL(glUniform1f(location, value));
}
void Shader::uniform(const std::string& name, const glm::vec2& value) {
    this->use();
    unsigned location = this->uniform_location(name);
    GL(glUniform2f(location, value.x, value.y));
}
void Shader::uniform(const std::string& name, const glm::vec3& value) {
    this->use();
    unsigned location = this->uniform_location(name);
    GL(glUniform3f(location, value.x, value.y, value.z));
}
void Shader::uniform(const std::string& name, const glm::vec4& value) {
    this->use();
    unsigned location = this->uniform_location(name);
    GL(glUniform4f(location, value.x, value.y, value.z, value.w));
}
void Shader::uniform(const std::string& name, int value) {
    this->use();
    unsigned location = this->uniform_location(name);
    GL(glUniform1i(location, value));
}
void Shader::uniform(const std::string& name, const glm::ivec2& value) {
    this->use();
    unsigned location = this->uniform_location(name);
    GL(glUniform2i(location, value.x, value.y));
}
void Shader::uniform(const std::string& name, const glm::ivec3& value) {
    this->use();
    unsigned location = this->uniform_location(name);
    GL(glUniform3i(location, value.x, value.y, value.z));
}
void Shader::uniform(const std::string& name, const glm::ivec4& value) {
    this->use();
    unsigned location = this->uniform_location(name);
    GL(glUniform4i(location, value.x, value.y, value.z, value.w));
}
void Shader::uniform(const std::string& name, const glm::mat4& value) {
    this->use();
    unsigned location = this->uniform_location(name);
    GL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
}

// private interface +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

unsigned Shader::uniform_location(const std::string& name) {
    unsigned location;

    // first check location cache
    auto it = m_uniform_location_cache.find(name);
    if (it != m_uniform_location_cache.end()) {
        return it->second;
    }

    // not found, query opengl and cache result
    GL(location = glGetUniformLocation(m_program_id, name.c_str()));
    if (location < 0) {
        std::stringstream ss;
        ss << "no uniform with name \'" << name << "\' found in shader \'";
        ss << m_program_name << "\'";
        throw std::runtime_error(ss.str());
    }
    m_uniform_location_cache[name] = location;

    return location;
}
