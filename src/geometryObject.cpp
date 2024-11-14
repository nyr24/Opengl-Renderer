#include "geometryObject.hpp"

my_gl::Geometry_object::Geometry_object(
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

my_gl::Geometry_object::Geometry_object(
    std::vector<my_gl::Animation<float>>&&     animations,
    std::size_t                                vertices_count,
    std::size_t                                buffer_byte_offset   
)
    : _animations{ std::move(animations) }
    , _vertices_count{ vertices_count }
    , _buffer_byte_offset{ buffer_byte_offset }
{}

my_gl::Geometry_object::Geometry_object(
    std::vector<my_gl_math::Matrix44<float>>&& matrices,
    std::size_t                                vertices_count,    std::size_t                                buffer_byte_offset   
)
    : _matrices{ std::move(matrices) }
    , _vertices_count{ vertices_count }
    , _buffer_byte_offset{ buffer_byte_offset }
{}

my_gl_math::Matrix44<float> my_gl::Geometry_object::get_local_mat() const {
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

void my_gl::Geometry_object::update_anims_time(Duration_sec frame_time) const {
    if (_animations.size() > 0) {
        for (auto& anim : _animations) {
            anim.update_time(frame_time);
        }
    }
}