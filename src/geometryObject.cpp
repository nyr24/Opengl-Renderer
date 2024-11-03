#include "geometryObject.hpp"

my_gl::Cube::Cube(
    std::vector<my_gl_math::Matrix44<float>>&& matrices,
    std::vector<my_gl::Animation<float>>&&     animations,
    std::size_t                                vertices_count,
    std::size_t                                buffer_byte_offset   
)
    : _matrices{ std::move(matrices) }
    , _animations{ std::move(animations) }
    , _vertices_count{ vertices_count }
    , _buffer_byte_offset{ buffer_byte_offset }
{}

my_gl::Cube::Cube(
    std::vector<my_gl::Animation<float>>&&     animations,
    std::size_t                                vertices_count,
    std::size_t                                buffer_byte_offset   
)
    : _animations{ std::move(animations) }
    , _vertices_count{ vertices_count }
    , _buffer_byte_offset{ buffer_byte_offset }
{}

my_gl::Cube::Cube(
    std::vector<my_gl_math::Matrix44<float>>&& matrices,
    std::size_t                                vertices_count,    std::size_t                                buffer_byte_offset   
)
    : _matrices{ std::move(matrices) }
    , _vertices_count{ vertices_count }
    , _buffer_byte_offset{ buffer_byte_offset }
{}

const my_gl_math::Matrix44<float> my_gl::Cube::get_local_mat() const {
    auto result_mat{ my_gl_math::Matrix44<float>::identity() };
    
    if (_matrices.size() > 0) {
        for (int i = 0; i < _matrices.size(); ++i) {
            result_mat *= _matrices[i];
        }
    }
    if (_animations.size() > 0) {
        for (int i = 0; i < _animations.size(); ++i) {
            result_mat *= const_cast<my_gl::Animation<float>&>(_animations[i]).update();
        }
    }

    return result_mat;
}

const my_gl_math::Matrix44<float> my_gl::Cube::get_local_mat(float curr_time) const {
    auto result_mat{ my_gl_math::Matrix44<float>::identity() };
    
    if (_matrices.size() > 0) {
        for (int i = 0; i < _matrices.size(); ++i) {
            result_mat *= _matrices[i];
        }
    }
    if (_animations.size() > 0) {
        for (int i = 0; i < _animations.size(); ++i) {
            result_mat *= const_cast<my_gl::Animation<float>&>(_animations[i]).update(curr_time);
        }
    }

    return result_mat;
}