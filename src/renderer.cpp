#include <iostream>
#include "renderer.hpp"
#include "utils.hpp"
#include "geometryObject.hpp"

my_gl::Program::Program(
    const char*                         vertex_shader_path,
    const char*                         fragment_shader_path,
    std::vector<my_gl::Attribute>&&     attribs
)
    : _program_id{ my_gl::create_program(vertex_shader_path, fragment_shader_path) }
{
    // set attributes
    for (my_gl::Attribute& attrib : attribs) {
        this->set_attrib(attrib);
    }
}

// uniforms provided
my_gl::Program::Program(
    const char*                         vertex_shader_path,
    const char*                         fragment_shader_path,
    std::vector<my_gl::Attribute>&&     attribs,
    std::vector<my_gl::Uniform>&&       unifs
)
    : _program_id{ my_gl::create_program(vertex_shader_path, fragment_shader_path) }
{
    // set attributes
    for (my_gl::Attribute& attrib : attribs) {
        this->set_attrib(attrib);
    }

    for (my_gl::Uniform& unif : unifs) {
        this->set_uniform(unif);
    }
}

my_gl::Program::~Program() {
    glDeleteProgram(_program_id);
}

const my_gl::Attribute* const my_gl::Program::get_attrib(std::string_view attrib_name) const {
    auto attr{ _attrs.find(attrib_name) };
    if (attr != _attrs.end()) {
        return &(attr->second);
    }
    else {
        return nullptr;
    }
}

const my_gl::Uniform* const my_gl::Program::get_uniform(std::string_view unif_name) const {
    auto unif{ _unifs.find(unif_name) };
    if (unif != _unifs.end() ) {
        return &(unif->second);
    }
    else {
        return nullptr;
    }
}

void my_gl::Program::set_attrib(my_gl::Attribute& attr) {
    if (_program_id == 0) {
        std::cerr << "program is not initialized, attribute: " << attr.name << " can't be set\n";
        return;
    }

    GLint attr_loc{ glGetAttribLocation(_program_id, attr.name) };

    if (attr_loc == -1) {
        std::cerr << "attribute name: " << attr.name << " wasn't found for program: " << _program_id << "\nnothing was set\n";
        return;
    }
    
    attr.location = attr_loc;

    _attrs[attr.name] = std::move(attr);
}

void my_gl::Program::set_uniform(my_gl::Uniform& unif) {
    if (_program_id == 0) {
        std::cerr << "program is not initialized, uniform: " << unif.name << " can't be set\n";
        return;
    }

    GLint unif_loc{ glGetUniformLocation(_program_id, unif.name) };

    if (unif_loc == -1) {
        std::cerr << "uniform name: " << unif.name << " wasn't found for program: " << _program_id << "\nnothing was set\n";
        return;
    }

    unif.location = unif_loc;

    _unifs[unif.name] = std::move(unif);
}

const std::unordered_map<std::string_view, my_gl::Attribute>& my_gl::Program::get_attrs() const {
    return _attrs;
}

const std::unordered_map<std::string_view, my_gl::Uniform>& my_gl::Program::get_unifs() const {
    return _unifs;
} 

// VertexArray
my_gl::VertexArray::VertexArray(
    std::vector<float>&& vbo_data,
    std::vector<uint16_t>&& ibo_data,
    const my_gl::Program& program
)
    : _vbo_data{ std::move(vbo_data) }
    , _ibo_data{ std::move(ibo_data) }
    , _program{ program }
{
    // vao
    glCreateVertexArrays(1, &_vao_id);
    glBindVertexArray(_vao_id);

    // vertex data
    glCreateBuffers(1, &_vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vbo_data.size(), _vbo_data.data(), GL_STATIC_DRAW);

    // indices
    glCreateBuffers(1, &_ibo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * _ibo_data.size(), _ibo_data.data(), GL_STATIC_DRAW);

    const auto& attrs = _program.get_attrs();

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

my_gl::VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &_vao_id);
    glDeleteBuffers(1, &_vbo_id);
    glDeleteBuffers(1, &_ibo_id);
}

// Renderer
my_gl::Renderer::Renderer(
    std::vector<IGeometry_object*>&&    objects,
    my_gl_math::Matrix44<float>&&       projection_view_mat,
    my_gl::Program&                     program,
    my_gl::VertexArray&                 vertex_arr
) 
    : _objects{ std::move(objects) }
    , _projection_view_mat{ std::move(projection_view_mat) }
    , _program{ program }
    , _vertex_arr{ vertex_arr }
{}

void my_gl::Renderer::render() const {
    for (const auto* obj : _objects) {
        const auto local_mat{ obj->get_local_mat() };
        my_gl_math::Matrix44<float> mvp_mat{ _projection_view_mat * local_mat };
        glUniformMatrix4fv(_program.get_uniform("u_mvp_mat")->location, 1, true, mvp_mat.data());
        glDrawElements(
            GL_TRIANGLES,
            obj->get_vertices_count(), 
            GL_UNSIGNED_SHORT, 
            reinterpret_cast<const void*>(obj->get_buffer_byte_offset())
        );
    }
}

void my_gl::Renderer::update_time(Duration_sec frame_duration) const {
    for (const auto* obj : _objects) {
        obj->update_anims_time(frame_duration);
    }
}