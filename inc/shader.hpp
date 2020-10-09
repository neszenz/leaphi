#pragma once

#include <glm/glm.hpp>

#include <map>
#include <string>

class Shader {
    public:
        Shader(const char* name, const char* v_path, const char* f_path);
        ~Shader();

        void use() const;

        void uniform(const std::string& name, float value);
        void uniform(const std::string& name, const glm::vec2& value);
        void uniform(const std::string& name, const glm::vec3& value);
        void uniform(const std::string& name, const glm::vec4& value);
        void uniform(const std::string& name, int value);
        void uniform(const std::string& name, const glm::ivec2& value);
        void uniform(const std::string& name, const glm::ivec3& value);
        void uniform(const std::string& name, const glm::ivec4& value);
        void uniform(const std::string& name, const glm::mat4& value);
    private:
        unsigned m_program_id;
        std::string m_program_name;
        std::map<std::string, unsigned> m_uniform_location_cache;

        unsigned uniform_location(const std::string& name);
};
