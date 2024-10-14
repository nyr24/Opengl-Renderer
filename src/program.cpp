#include <iostream>
#include "program.hpp"
#include "utils.hpp"

my_gl::Program::Program(const char* vertex_shader_path, const char* fragment_shader_path)
    : _prog{ my_gl::createProgram(vertex_shader_path, fragment_shader_path) }
{}

GLint my_gl::Program::get_attrib_loc(const char* attrib_name) const {
    const my_gl::Attribute& attr = _attrs.at(attrib_name);
    return attr.location;
}

GLint my_gl::Program::get_uniform_loc(const char* unif_name) const {
    const my_gl::Uniform& unif = _unifs.at(unif_name);
    return unif.location;
}

void my_gl::Program::set_attrib(const char* attr_name) {
    if (_prog == 0) {
        std::cerr << "program is not initialized, attribute: " << attr_name << " can't be set\n";
        return;
    }

    GLint attr_loc{ glGetAttribLocation(_prog, attr_name) };

    if (attr_loc == -1) {
        std::cerr << "attribute name: " << attr_name << " wasn't found for program: " << _prog << "\nnothing was set\n";
        return;
    }
    
    _attrs[attr_name] = my_gl::Attribute{ .name = attr_name, .location = attr_loc };
}

void my_gl::Program::set_uniform(const char* unif_name) {
    if (_prog == 0) {
        std::cerr << "program is not initialized, uniform: " << unif_name << " can't be set\n";
        return;
    }

    GLint unif_loc{ glGetUniformLocation(_prog, unif_name) };

    if (unif_loc == -1) {
        std::cerr << "uniform name: " << unif_name << " wasn't found for program: " << _prog << "\nnothing was set";
        return;
    }

    _unifs[unif_name] = my_gl::Uniform{ .name = unif_name, .location = unif_loc };
}

void my_gl::Program::use() const { 
    glUseProgram(_prog);
}

void my_gl::Program::un_use() const { 
    glUseProgram(0);
}