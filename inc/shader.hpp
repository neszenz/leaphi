#pragma once

#include <glm/glm.hpp>

#include <map>
#include <string>

class Shader {
    public:
        Shader(const char* name, const char* v_path, const char* f_path);
        ~Shader();

        void use() const;

        void uniformMat4(const std::string& name, const glm::mat4& value);
    private:
        unsigned m_program_id;
        std::string m_program_name;
        std::map<std::string, unsigned> m_uniform_location_cache;

        unsigned get_uniform_location(const std::string& name);
};
