#include <iostream>
#include "renderer.hpp"
#include "utils.hpp"
#include "geometryObject.hpp"
#include "matrix.hpp"
#include "sharedTypes.hpp"

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
        this->set_uniform_location(unif);
    }
}

my_gl::Program::~Program() {
    un_use();
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

void my_gl::Program::set_uniform_location(my_gl::Uniform& unif) {
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

#ifdef DEBUG
    std::cout << "uniform " << unif.name << " location is assigned to " << unif_loc << '\n';
#endif // DEBUG

    _unifs[unif.name] = std::move(unif);
}

void  my_gl::Program::set_uniform_value(std::string_view unif_name, int32_t val) const {
    use();
    const Uniform* unif{ get_uniform(unif_name) };
    if (!unif) {
        return;
    }
    glUniform1i(unif->location, val);
}

void my_gl::Program::set_uniform_value(std::string_view unif_name, float val) const {
    use();
    const Uniform* unif{ get_uniform(unif_name) };
    if (!unif) {
        return;
    }
    glUniform1f(unif->location, val);
}

void my_gl::Program::set_uniform_value(std::string_view unif_name, float val1, float val2, float val3) const {
    use();
    const Uniform* unif{ get_uniform(unif_name) };
    if (!unif) {
        return;
    }
    glUniform3f(unif->location, val1, val2, val3);
}

void my_gl::Program::set_uniform_value(std::string_view unif_name, const float* matrix_val) const {
    use();
    const Uniform* unif{ get_uniform(unif_name) };
    if (!unif) {
        return;
    }
    glUniformMatrix4fv(unif->location, 1, true, matrix_val);
}

void my_gl::Program::set_uniform_value(std::string_view unif_name, const my_gl::math::Vec3<float>& vec3_val) const
{
    use();
    const Uniform* unif{ get_uniform(unif_name) };
    if (!unif) {
        return;
    }
    const float* vec_ptr = std::begin(vec3_val._data);
    glUniform3fv(unif->location, 1, vec_ptr);
}

void my_gl::Program::set_uniform_value(std::string_view unif_name, const my_gl::math::Vec4<float>& vec4_val) const
{
    use();
    const Uniform* unif{ get_uniform(unif_name) };
    if (!unif) {
        return;
    }
    glUniform4f(unif->location, vec4_val[0], vec4_val[1], vec4_val[2], vec4_val[3]);
}

const std::unordered_map<std::string_view, my_gl::Attribute>& my_gl::Program::get_attrs() const {
    return _attrs;
}

const std::unordered_map<std::string_view, my_gl::Uniform>& my_gl::Program::get_unifs() const {
    return _unifs;
} 

// VertexArray
my_gl::VertexArray::VertexArray(
    meshes::Mesh&& mesh,
    const Program& program
)
    : _vbo_data{ std::move(mesh.vertices) }
    , _ibo_data{ std::move(mesh.indices) }
{
    init(program);
}

my_gl::VertexArray::VertexArray(
    const meshes::Mesh& mesh,
    const Program&      program
)
    : _vbo_data{ mesh.vertices }
    , _ibo_data{ mesh.indices }
{
    init(program);
}

my_gl::VertexArray::VertexArray(
    meshes::Mesh&& mesh,
    const std::vector<const Program*>& programs
)
    : _vbo_data{ std::move(mesh.vertices) }
    , _ibo_data{ std::move(mesh.indices) }
{
    init(programs);
}

my_gl::VertexArray::VertexArray(
    const meshes::Mesh& mesh,
    const std::vector<const Program*>& programs
)
    : _vbo_data{ mesh.vertices }
    , _ibo_data{ mesh.indices }
{
    init(programs);
}

my_gl::VertexArray::VertexArray(
    const std::vector<meshes::Mesh>& meshes,
    const std::vector<const Program*>& programs
)
{
    combine_meshes(meshes);
    init(programs);
}

void my_gl::VertexArray::combine_meshes(const std::vector<meshes::Mesh>& meshes) {
    size_t vbo_data_size{0};
    size_t ibo_data_size{0};

    for (const meshes::Mesh& mesh : meshes) {
        vbo_data_size += mesh.vertices.size();
        ibo_data_size += mesh.indices.size();
    }

    _vbo_data.reserve(vbo_data_size);
    _ibo_data.reserve(ibo_data_size);

    for (const meshes::Mesh& mesh : meshes) {
        _vbo_data.insert(_vbo_data.end(), mesh.vertices.begin(), mesh.vertices.end());
        _ibo_data.insert(_ibo_data.end(), mesh.indices.begin(), mesh.indices.end());
    }
}

my_gl::VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &_vao_id);
    glDeleteBuffers(1, &_vbo_id);
    glDeleteBuffers(1, &_ibo_id);
}

void my_gl::VertexArray::init(const Program& program) {
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

    const auto& attrs{ program.get_attrs() };

    for (auto it{ attrs.begin() }; it != attrs.end(); ++it) {
        const my_gl::Attribute& attr_ref{ it->second };

        glEnableVertexAttribArray(attr_ref.location);
        glVertexAttribPointer(attr_ref.location, attr_ref.count, attr_ref.gl_type, false, attr_ref.byte_stride, reinterpret_cast<void*>(attr_ref.byte_offset));

#ifdef DEBUG
        printf("info: attribute '%s' successfully initialized, location: '%d'\n", attr_ref.name, attr_ref.location);
#endif
    }

    // unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void my_gl::VertexArray::init(const std::vector<const Program*>& programs) {
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

    for (const auto* program : programs) {
        const auto& attrs = program->get_attrs();

        for (auto it = attrs.begin(); it != attrs.end(); ++it) {
            const my_gl::Attribute& attr_ref{ it->second };

            glEnableVertexAttribArray(attr_ref.location);
            glVertexAttribPointer(attr_ref.location, attr_ref.count, attr_ref.gl_type, false, attr_ref.byte_stride, reinterpret_cast<void*>(attr_ref.byte_offset));

#ifdef DEBUG
            printf("info: attribute '%s' successfully initialized, location: '%d'\n", attr_ref.name, attr_ref.location);
#endif
        }
    }

    // unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Renderer
my_gl::Renderer::Renderer(
    std::span<my_gl::GeometryObjectComplex>     complex_objs,
    std::span<my_gl::GeometryObjectPrimitive>   primitives,
    math::Matrix44<float>&&                     view_mat,
    math::Matrix44<float>&&                     proj_mat
)
    : _complex_objs{ complex_objs }
    , _primitives{ primitives }
    , _view_mat{ std::move(view_mat) }
    , _proj_mat{ std::move(proj_mat) }
{}

void my_gl::Renderer::render(float time_0to1) {
    auto view_proj_mat{ _proj_mat * _view_mat };

    for (auto& complex_obj : _complex_objs) {
        complex_obj.render(_view_mat, view_proj_mat, time_0to1);
    }

    for (auto& primitive : _primitives) {
        primitive.render(_view_mat, view_proj_mat, time_0to1);
    }
}

void my_gl::Renderer::update_time(Duration_sec frame_duration) {
    _rendering_time_curr += frame_duration;

    for (auto& complex_obj : _complex_objs) {
        complex_obj.update_anims_time(frame_duration);
    }

    for (auto& primitive : _primitives) {
        primitive.update_anims_time(frame_duration);
    }
}

my_gl::Duration_sec my_gl::Renderer::get_curr_rendering_duration() const {
    return _rendering_time_curr - _rendering_time_start;
}
