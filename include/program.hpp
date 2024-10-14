#pragma once
#include <unordered_map>
#include <GLEW/glew.h>

namespace my_gl { 
    struct Attribute {
        const char* name;
        GLint location;
    };

    struct Uniform {
        const char* name;
        GLint location;
    };

    class Program {
    public:
        Program(const char* vertex_shader_path, const char* fragment_shader_path);

        GLint get_attrib_loc(const char* attrib_name) const;
        GLint get_uniform_loc(const char* unif_name) const;
        void set_attrib(const char* name);
        void set_uniform(const char* name);
        void use() const;
        void un_use() const;

    private:
        GLuint _prog{ 0 };
        std::unordered_map<const char*, Attribute> _attrs;
        std::unordered_map<const char*, Uniform> _unifs;
    };
}