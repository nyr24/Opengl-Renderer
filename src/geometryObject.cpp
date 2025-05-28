#include <span>
#include "geometryObject.hpp"
#include "animation.hpp"
#include "matrix.hpp"
#include "meshes.hpp"
#include "renderer.hpp"
#include "sharedTypes.hpp"
#ifdef DEBUG
#include <iostream>
#endif

my_gl::TransformGroup::TransformGroup(
    my_gl::math::TransformationType                 arg_type,
    std::vector<math::Matrix44<float>>&&            arg_transforms,
    std::vector<my_gl::Animation<float>>&&          arg_anims
)
    : type{ arg_type }
    , transforms{ std::move(arg_transforms) }
    , anims{ std::move(arg_anims) }
{}

my_gl::GeometryObjectPrimitive::GeometryObjectPrimitive(
    std::span<my_gl::TransformGroup>    transforms,
    Velocity<float>&&                   velocity,
    float                               mass,
    std::size_t                         vertices_count,
    std::size_t                         buffer_byte_offset,
    const Program&                      program,
    const VertexArray&                  vao,
    GLenum                              draw_type,
    Material::Type                      material_type,
    const Texture* const                texture
)
    : _transforms{ transforms }
    , _velocity(std::move(velocity))
    , _texture{ texture }
    , _program{ program }
    , _vao{ vao }
    , _vertices_count{ vertices_count }
    , _buffer_byte_offset{ buffer_byte_offset }
    , _mass{ mass }
    , _draw_type{ draw_type }
    , _material_type{ material_type }
{}

void my_gl::GeometryObjectPrimitive::calc_model_mat_frame(Duration_sec frame_time) {
    auto result_mat{ my_gl::math::Matrix44<float>::identity_new() };

    if (_transforms.size()) {
        for (my_gl::TransformGroup& transforms_by_type : _transforms) {
            if (transforms_by_type.type == math::TransformationType::TRANSLATION) {
                for (const auto& transform : transforms_by_type.transforms) {
                    result_mat *= transform;
                }
                result_mat *= _velocity.update(frame_time.count());
                for (my_gl::Animation<float>& animation : transforms_by_type.anims) {
                    result_mat *= animation.update();
                }
            } else {
                for (const auto& transform : transforms_by_type.transforms) {
                    result_mat *= transform;
                }
                for (my_gl::Animation<float>& animation : transforms_by_type.anims) {
                    result_mat *= animation.update();
                }
            }
        }
    }

    _model_mat = std::move(result_mat);
}

void my_gl::GeometryObjectPrimitive::update_anims_time(Duration_sec frame_time) {
    for (auto& transform_by_type : _transforms) {
        for (my_gl::Animation<float>& anim : transform_by_type.anims) {
            anim.update_time(frame_time);
        }
    }
}

void my_gl::GeometryObjectPrimitive::bind_state() const {
    if (_texture) {
        _texture->bind();
    }
    _program.use();
    _vao.bind();
}

void my_gl::GeometryObjectPrimitive::un_bind_state() const {
    if (_texture) {
        _texture->un_bind();
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
    Duration_sec frame_time,
    float time_0to1
)
{
    bind_state();

    this->calc_model_mat_frame(frame_time);
    // my_gl::math::Matrix44<float> model_mat{ get_model_mat(delta_time) };
    my_gl::math::Matrix44<float> model_view_mat{ view_mat * _model_mat };
    my_gl::math::Matrix44<float> normal_mat{ model_view_mat.invert().transpose() };
    my_gl::math::Matrix44<float> mvp_mat{ view_proj_mat * _model_mat };

    _program.set_uniform_value("u_model_view_mat", model_view_mat.data());
    _program.set_uniform_value("u_normal_mat", normal_mat.data());
    _program.set_uniform_value("u_mvp_mat", mvp_mat.data());
    // _program.set_uniform_value("u_lerp", time_0to1);

    if (_material_type != Material::NO_MATERIAL) {
        const my_gl::Material& material = my_gl::Material::get_from_table(_material_type);
        _program.set_uniform_value("u_material.ambient", material.ambient);
        _program.set_uniform_value("u_material.diffuse", material.diffuse);
        _program.set_uniform_value("u_material.specular", material.specular);
        _program.set_uniform_value("u_material.shininess", material.shininess);
    }

    draw();
    un_bind_state();
}

bool my_gl::GeometryObjectPrimitive::check_collision(GeometryObjectPrimitive& second) {
    if (this == &second) {
        return false;
    }

    meshes::Boundaries first_transformed_boundaries{ this->_vao._mesh.transform_boundaries(_model_mat) };
    meshes::Boundaries second_transformed_boundaries{ second._vao._mesh.transform_boundaries(second._model_mat) };

    // TODO: checking if the sides were reversed by rotations
    // float first_left = first_transformed_boundaries.ltn[0] < first_transformed_boundaries.rtn[0]
    //     ? first_transformed_boundaries.ltn[0] : first_transformed_boundaries.rtn[0];
    // float first_top = first_transformed_boundaries.ltn[1] < first_transformed_boundaries.rtn[1]
    //     ? first_transformed_boundaries.ltn[1] : first_transformed_boundaries.rtn[1];
    // float first_near = first_transformed_boundaries.ltn[2] < first_transformed_boundaries.rtn[2]
    //     ? first_transformed_boundaries.ltn[2] : first_transformed_boundaries.rtn[2];
    //
    // float second_left = second_transformed_boundaries.ltn[0] < second_transformed_boundaries.rtn[0]
    //     ? second_transformed_boundaries.ltn[0] : second_transformed_boundaries.rtn[0];
    // float second_top = second_transformed_boundaries.ltn[1] < second_transformed_boundaries.rtn[1]
    //     ? second_transformed_boundaries.ltn[1] : second_transformed_boundaries.rtn[1];
    // float second_near = second_transformed_boundaries.ltn[2] < second_transformed_boundaries.rtn[2]
    //     ? second_transformed_boundaries.ltn[2] : second_transformed_boundaries.rtn[2];

    // determine which object is left, top, near
    meshes::Boundaries& left_object = first_transformed_boundaries.ltn[0] < second_transformed_boundaries.ltn[0] ? first_transformed_boundaries : second_transformed_boundaries;
    meshes::Boundaries& right_object = first_transformed_boundaries.ltn[0] > second_transformed_boundaries.ltn[0] ? first_transformed_boundaries : second_transformed_boundaries;
    meshes::Boundaries& top_object = first_transformed_boundaries.ltn[1] > second_transformed_boundaries.ltn[1] ? first_transformed_boundaries : second_transformed_boundaries;
    meshes::Boundaries& bottom_object = first_transformed_boundaries.ltn[1] < second_transformed_boundaries.ltn[1] ? first_transformed_boundaries : second_transformed_boundaries;
    meshes::Boundaries& near_object = first_transformed_boundaries.ltn[2] < second_transformed_boundaries.ltn[2] ? first_transformed_boundaries : second_transformed_boundaries;
    meshes::Boundaries& far_object = first_transformed_boundaries.ltn[2] > second_transformed_boundaries.ltn[2] ? first_transformed_boundaries : second_transformed_boundaries;

    return left_object.rtn[0] >= right_object.ltn[0] && bottom_object.rtn[1] >= top_object.rbn[1] && near_object.ltf[2] >= far_object.ltn[2];
}

void my_gl::GeometryObjectPrimitive::handle_collision(my_gl::GeometryObjectPrimitive& second) {
    // this->_velocity._velocity = ((this->_mass - second._mass) * this->_velocity._velocity + 2 * second._mass * second._velocity._velocity) / (this->_mass + second._mass);
    // second._velocity._velocity = ((second._mass - this->_mass) * second._velocity._velocity + 2 * this->_mass * this->_velocity._velocity) / (this->_mass + second._mass);
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
    Duration_sec frame_time,
    float time_0to1
)
{
    for (auto& primitive : _primitives) {
        primitive.render(view_mat, view_proj_mat, frame_time, time_0to1);
    }
}

void my_gl::GeometryObjectComplex::update_anims_time(my_gl::Duration_sec frame_time)
{
    for (auto& primitive : _primitives) {
        primitive.update_anims_time(frame_time);
    }
}

// Material
my_gl::Material::Material(
    my_gl::math::Vec3<float>&&      ambient_,
    my_gl::math::Vec3<float>&&      diffuse_,
    my_gl::math::Vec3<float>&&      specular_,
    float                           shininess_
)
    : ambient(std::move(ambient_))
    , diffuse(std::move(diffuse_))
    , specular(std::move(specular_))
    , shininess(shininess_)
{}

const std::array<my_gl::Material, my_gl::Material::COUNT> my_gl::material_table = {
    // NOTE: test material
    // my_gl::Material({ 1.0f, 0.5f, 0.31f }, { 1.0f, 0.5f, 0.31f }, { 0.5f, 0.5f, 0.5f }, 32.0f),

    my_gl::Material({ 0.0215, 0.1745, 0.0215 }, { 0.07568, 0.61424, 0.07568 }, { 0.633, 0.727811, 0.633 }, 0.6f),
    my_gl::Material({ 0.05375, 0.05, 0.06625 }, { 0.18275, 0.17, 0.22525 }, { 0.332741, 0.328634, 0.346435 }, 0.3f),
    my_gl::Material({ 0.1745, 0.01175, 0.01175 }, { 0.61424, 0.04136, 0.04136 }, { 0.727811, 0.626959, 0.626959 }, 0.6f),
    my_gl::Material({ 0.1, 0.18725, 0.1745 }, { 0.396, 0.74151, 0.69102 }, { 0.297254, 0.30829, 0.306678 }, 0.1f),
    my_gl::Material({ 0.24725, 0.1995, 0.0745 }, { 0.75164, 0.60648, 0.22648 }, { 0.628281, 0.555802, 0.366065 }, 0.4f),
    my_gl::Material({ 0.0, 0.1, 0.06 }, { 0.0, 0.50980392, 0.50980392 }, { 0.50196078, 0.50196078, 0.50196078 }, 0.25f),
    my_gl::Material({ 0.0, 0.05, 0.0 }, { 0.4, 0.5, 0.4 }, { 0.04, 0.7, 0.04 }, 0.078125f),
};

const my_gl::Material& my_gl::Material::get_from_table(Type material_type) {
    assert(material_type >= 0 && material_type < my_gl::material_table.size() && "Invalid material type");
    return my_gl::material_table[material_type];
}
