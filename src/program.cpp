#include <iostream>
#include "program.hpp"
#include "utils.hpp"

my_gl::Program::Program(
    const char*                         vertex_shader_path,
    const char*                         fragment_shader_path,
    std::vector<my_gl::Attribute>&&     attribs,
    // forwarded to VertexData
    std::vector<float>&&                vbo_data,
    std::vector<uint16_t>&&             ibo_data
)
    : _prog_id{ my_gl::createProgram(vertex_shader_path, fragment_shader_path) }
    , _vertex_data{
        std::move(vbo_data),
        std::move(ibo_data)
    }
{
    // set attributes
    for (my_gl::Attribute& attrib : attribs) {
        this->set_attrib(attrib);
    }

    _vertex_data.init(this);
}

// uniforms provided
my_gl::Program::Program(
    const char*                         vertex_shader_path,
    const char*                         fragment_shader_path,
    std::vector<my_gl::Attribute>&&     attribs,
    std::vector<my_gl::Uniform>&&       unifs,
    // forwarded to VertexData
    std::vector<float>&&                vbo_data,
    std::vector<uint16_t>&&             ibo_data
)
    : _prog_id{ my_gl::createProgram(vertex_shader_path, fragment_shader_path) }
    , _vertex_data{
        std::move(vbo_data),
        std::move(ibo_data)
    }
{
    // set attributes
    for (my_gl::Attribute& attrib : attribs) {
        this->set_attrib(attrib);
    }

    for (my_gl::Uniform& unif : unifs) {
        this->set_uniform(unif);
    }

    _vertex_data.init(this);
}

my_gl::Program::~Program() {
    glDeleteProgram(_prog_id);
}

const my_gl::Attribute* const my_gl::Program::get_attrib(const char* attrib_name) const {
    auto attr{ _attrs.find(attrib_name) };
    if (attr != _attrs.end()) {
        return &(attr->second);
    }
    else {
        return nullptr;
    }
}

const my_gl::Uniform* const my_gl::Program::get_uniform(const char* unif_name) const {
    auto unif{ _unifs.find(unif_name) };
    if (unif != _unifs.end() ) {
        return &(unif->second);
    }
    else {
        return nullptr;
    }
}

void my_gl::Program::set_attrib(my_gl::Attribute& attr) {
    if (_prog_id == 0) {
        std::cerr << "program is not initialized, attribute: " << attr.name << " can't be set\n";
        return;
    }

    GLint attr_loc{ glGetAttribLocation(_prog_id, attr.name) };

    if (attr_loc == -1) {
        std::cerr << "attribute name: " << attr.name << " wasn't found for program: " << _prog_id << "\nnothing was set\n";
        return;
    }
    
    attr.location = attr_loc;

    _attrs[attr.name] = std::move(attr);
}

void my_gl::Program::set_uniform(my_gl::Uniform& unif) {
    if (_prog_id == 0) {
        std::cerr << "program is not initialized, uniform: " << unif.name << " can't be set\n";
        return;
    }

    GLint unif_loc{ glGetUniformLocation(_prog_id, unif.name) };

    if (unif_loc == -1) {
        std::cerr << "uniform name: " << unif.name << " wasn't found for program: " << _prog_id << "\nnothing was set";
        return;
    }

    unif.location = unif_loc;

    _unifs[unif.name] = std::move(unif);
}

const std::unordered_map<const char*, my_gl::Attribute>& my_gl::Program::get_attrs() const {
    return _attrs;
}

const std::unordered_map<const char*, my_gl::Uniform>& my_gl::Program::get_unifs() const {
    return _unifs;
} 

// VertexData
my_gl::VertexData::VertexData(
    std::vector<float>&& vbo_data,
    std::vector<uint16_t>&& ibo_data
)
    : _vbo_data{ std::move(vbo_data) }
    , _ibo_data{ std::move(ibo_data) }
{}

my_gl::VertexData::~VertexData() {
    glDeleteVertexArrays(1, &_vao_id);
    glDeleteBuffers(1, &_vbo_id);
    glDeleteBuffers(1, &_ibo_id);
}

void my_gl::VertexData::init(const Program* program) {
    _program = program;

    // vao
    glCreateVertexArrays(1, &_vao_id);
    glBindVertexArray(_vao_id);

    // vertex data
    glCreateBuffers(1, &_vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vbo_data.size(), _vbo_data.data(), GL_STATIC_DRAW);

    // indeces
    glCreateBuffers(1, &_ibo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * _ibo_data.size(), _ibo_data.data(), GL_STATIC_DRAW);

    const auto& attrs = _program->get_attrs();

    for (auto it = attrs.begin(); it != attrs.end(); ++it) {
        const my_gl::Attribute& attr_ref{ it->second };

        glEnableVertexAttribArray(attr_ref.location);
        glVertexAttribPointer(attr_ref.location, attr_ref.count, attr_ref.gl_type, false, attr_ref.byte_stride, reinterpret_cast<void*>(attr_ref.byte_offset));

        printf("info: attribute '%s' successfully initialized, location: '%d'\n", attr_ref.name, attr_ref.location);
    }

    // unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}