#include "geometryObject.hpp"
#include "animation.hpp"
#include "matrix.hpp"
#include "renderer.hpp"
#include "sharedTypes.hpp"
#include <optional>

my_gl::TransformsByType::TransformsByType(
    my_gl::math::TransformationType                 arg_type,
    std::vector<math::Transformation<float>>&&      arg_transforms,
    std::vector<my_gl::Animation<float>>&&          arg_anims
)
    : type{ arg_type }
    , transforms{ std::move(arg_transforms) }
    , anims{ std::move(arg_anims) }
{}

my_gl::GeometryObjectPrimitive::GeometryObjectPrimitive(
    std::vector<my_gl::TransformsByType>&&              transforms,
    std::size_t                                         vertices_count,
    std::size_t                                         buffer_byte_offset,
    const Program&                                      program,
    const VertexArray&                                  vao,
    GLenum                                              draw_type,
    std::vector<const my_gl::Texture*>&&                textures = {}
)
    : _transforms{ std::move(transforms) }
    , _textures{ std::move(textures) }
    , _vertices_count{ vertices_count }
    , _buffer_byte_offset{ buffer_byte_offset }
    , _program{ program }
    , _vao{ vao }
    , _draw_type{ draw_type }
{}

my_gl::math::Matrix44<float> my_gl::GeometryObjectPrimitive::get_model_mat() {
    auto result_mat{ my_gl::math::Matrix44<float>::identity_new() };

    for (my_gl::TransformsByType& transforms_by_type : _transforms) {
        for (const auto& transform : transforms_by_type.transforms) {
            result_mat *= transform._inner_mat;
        }
        for (my_gl::Animation<float>& animation : transforms_by_type.anims) {
            result_mat *= animation.update();
        }
    }

    return result_mat;
}

void my_gl::GeometryObjectPrimitive::update_anims_time(Duration_sec frame_time) {
    for (auto& transform_by_type : _transforms) {
        for (my_gl::Animation<float>& anim : transform_by_type.anims) {
            anim.update_time(frame_time);
        }
    }
}

void my_gl::GeometryObjectPrimitive::bind_state() const {
    if (_textures.size()) {
        for (const auto* texture : _textures) {
            texture->bind();
        }
    }
    _program.use();
    _vao.bind();
}

void my_gl::GeometryObjectPrimitive::un_bind_state() const {
    if (_textures.size()) {
        for (const auto* texture : _textures) {
            texture->un_bind();
        }
    }
    _program.un_use();
    _vao.un_bind();
}

void my_gl::GeometryObjectPrimitive::draw() const {
    glDrawElements(
        _draw_type,
        _vertices_count,
        GL_UNSIGNED_SHORT,
        reinterpret_cast<const void*>(_buffer_byte_offset)
    );
}

void my_gl::GeometryObjectPrimitive::render(
    const my_gl::math::Matrix44<float>& view_mat,
    const my_gl::math::Matrix44<float>& view_proj_mat,
    float time_0to1)
{
    bind_state();

    const Program& shader{ get_program() };

    my_gl::math::Matrix44<float> model_mat{ get_model_mat() };
    my_gl::math::Matrix44<float> model_view_mat{ view_mat * model_mat };
    my_gl::math::Matrix44<float> normal_mat{ model_view_mat.invert().transpose() };
    my_gl::math::Matrix44<float> mvp_mat{ view_proj_mat * model_mat };

    shader.set_uniform_value("u_model_view_mat", model_view_mat.data());
    shader.set_uniform_value("u_normal_mat", normal_mat.data());
    shader.set_uniform_value("u_mvp_mat", mvp_mat.data());
    shader.set_uniform_value("u_lerp", time_0to1);

    draw();
    un_bind_state();
}

// GeometryObjectComplex
my_gl::GeometryObjectComplex::GeometryObjectComplex(
    std::vector<my_gl::GeometryObjectPrimitive>&& primitives
)
    : _primitives{ std::move(primitives) }
{}

my_gl::GeometryObjectComplex::GeometryObjectComplex(
    const std::vector<my_gl::GeometryObjectPrimitive>& primitives
)
    : _primitives{ primitives }
{}

void my_gl::GeometryObjectComplex::render(
    const my_gl::math::Matrix44<float>& view_mat,
    const my_gl::math::Matrix44<float>& view_proj_mat,
    float time_0to1
)
{
    for (auto& primitive : _primitives) {
        primitive.render(view_mat, view_proj_mat, time_0to1);
    }
}

void my_gl::GeometryObjectComplex::update_anims_time(my_gl::Duration_sec frame_time)
{
    for (auto& primitive : _primitives) {
        primitive.update_anims_time(frame_time);
    }
}
