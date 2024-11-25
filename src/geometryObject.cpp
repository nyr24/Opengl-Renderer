#include "geometryObject.hpp"
#include "renderer.hpp"

my_gl::GeometryObject::GeometryObject(
    std::vector<my_gl_math::Matrix44<float>>&& matrices,
    std::vector<my_gl::Animation<float>>&&     animations,
    std::size_t                                vertices_count,
    std::size_t                                buffer_byte_offset,
    const Program&                             program,
    const VertexArray&                         vao,
    GLenum                                     draw_type,
    std::vector<const my_gl::Texture*>&&       textures = { nullptr }
)
    : _matrices{ std::move(matrices) }
    , _animations{ std::move(animations) }
    , _textures{ std::move(textures) }
    , _vertices_count{ vertices_count }
    , _buffer_byte_offset{ buffer_byte_offset }
    , _program{ program } 
    , _vao{ vao }
    , _draw_type{ draw_type }
{}

my_gl::GeometryObject::GeometryObject(
    std::vector<my_gl::Animation<float>>&&     animations,
    std::size_t                                vertices_count,
    std::size_t                                buffer_byte_offset,
    const Program&                             program,
    const VertexArray&                         vao,
    GLenum                                     draw_type,
    std::vector<const my_gl::Texture*>&&       textures = { nullptr }
)
    : _animations{ std::move(animations) }
    , _vertices_count{ vertices_count }
    , _textures{ std::move(textures) }
    , _buffer_byte_offset{ buffer_byte_offset }
    , _program{ program } 
    , _vao{ vao }
    , _draw_type{ draw_type }
{}

my_gl::GeometryObject::GeometryObject(
    std::vector<my_gl_math::Matrix44<float>>&& matrices,
    std::size_t                                vertices_count,  
    std::size_t                                buffer_byte_offset,
    const Program&                             program,
    const VertexArray&                         vao,
    GLenum                                     draw_type,
    std::vector<const my_gl::Texture*>&&       textures = { nullptr }
)
    : _matrices{ std::move(matrices) }
    , _vertices_count{ vertices_count }
    , _textures{ std::move(textures) }
    , _buffer_byte_offset{ buffer_byte_offset }
    , _program{ program } 
    , _vao{ vao }
    , _draw_type{ draw_type }
{}

my_gl_math::Matrix44<float> my_gl::GeometryObject::get_local_mat() const {
    auto result_mat{ my_gl_math::Matrix44<float>::identity() };
    
    if (_matrices.size() > 0) {
        for (const auto& curr_mat : _matrices) {
            result_mat *= curr_mat;
        }
    }
    if (_animations.size() > 0) {
        for (auto& curr_anim : _animations) {
            result_mat *= curr_anim.update();
        }
    }

    return result_mat;
}

void my_gl::GeometryObject::update_anims_time(Duration_sec frame_time) const {
    if (_animations.size() > 0) {
        for (auto& anim : _animations) {
            anim.update_time(frame_time);
        }
    }
}

void my_gl::GeometryObject::bind_state() const {
    if (_textures.size() > 0) {
        for (const auto* texture : _textures) {
            texture->bind();
        }
    }
    _program.use();
    _vao.bind();
}

void my_gl::GeometryObject::un_bind_state() const {
    if (_textures.size() > 0) {
        for (const auto* texture : _textures) {
            texture->un_bind();
        }
    }
    _program.un_use();
    _vao.un_bind();
}

void my_gl::GeometryObject::draw() const {
    glDrawElements(
        _draw_type,
        _vertices_count,
        GL_UNSIGNED_SHORT,
        reinterpret_cast<const void*>(_buffer_byte_offset)
    );
}