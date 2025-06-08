#include "geometryObject.hpp"
#include "animation.hpp"
#include "matrix.hpp"
#include "meshes.hpp"
#include "renderer.hpp"
#include "sharedTypes.hpp"
#include "vec.hpp"
#include <cstdio>
#include <span>
#include <utility>
#ifdef DEBUG
#include <iostream>
#endif

my_gl::TransformData::TransformData(
    my_gl::math::TransformationType                 arg_type,
    std::vector<math::Matrix44<float>>&&            arg_transforms,
    std::vector<my_gl::Animation<float>>&&          arg_anims
)
    : type{ arg_type }
    , transforms{ std::move(arg_transforms) }
    , anims{ std::move(arg_anims) }
{}

my_gl::GeometryObjectPrimitive::GeometryObjectPrimitive(
    std::span<my_gl::TransformData>     transform_data,
    Physics<float>* const               physics,
    std::size_t                         vertices_count,
    std::size_t                         buffer_byte_offset,
    const Program&                      program,
    const VertexArray&                  vao,
    GLenum                              draw_type,
    Material::Type                      material_type,
    CollisionType                       collision_type,
    const Texture* const                texture
)
    : _transform_data{ transform_data }
    , _physics{ physics }
    , _texture{ texture }
    , _program{ program }
    , _vao{ vao }
    , _vertices_count{ vertices_count }
    , _buffer_byte_offset{ buffer_byte_offset }
    , _model_mat{ my_gl::math::Matrix44<float>::identity_new() }
    , _draw_type{ draw_type }
    , _material_type{ material_type }
    , _collision_type{ collision_type }
{}

void my_gl::GeometryObjectPrimitive::calc_model_mat_frame(Duration_sec frame_time) {
    auto result_mat{ my_gl::math::Matrix44<float>::identity_new() };

    if (_transform_data.size()) {
        for (my_gl::TransformData& transforms_by_type : _transform_data) {
            if (transforms_by_type.type == math::TransformationType::TRANSLATION && _collision_type != CollisionType::STATIC && _physics) {
                for (const auto& transform : transforms_by_type.transforms) {
                    result_mat *= transform;
                }
                result_mat *= _physics->update(frame_time.count());
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
    for (auto& transform_by_type : _transform_data) {
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

my_gl::CollisionResult my_gl::GeometryObjectPrimitive::check_collision(GeometryObjectPrimitive& second) {
    if (this == &second) {
        return {};
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
 
    CollisionResult coll_res;

    coll_res.is_left = first_transformed_boundaries.ltn[0] < second_transformed_boundaries.ltn[0];
    coll_res.is_bottom = first_transformed_boundaries.lbn[1] < second_transformed_boundaries.lbn[1];
    coll_res.is_near = first_transformed_boundaries.ltn[2] < second_transformed_boundaries.ltn[2];

    if (coll_res.is_left) {
        coll_res.offset_x = second_transformed_boundaries.ltn[0] - first_transformed_boundaries.rtn[0];
    } else {
        coll_res.offset_x = first_transformed_boundaries.ltn[0] - second_transformed_boundaries.rtn[0];
    }

    if (coll_res.is_bottom) {
        coll_res.offset_y = second_transformed_boundaries.lbn[1] - first_transformed_boundaries.ltn[1];
    } else {
        coll_res.offset_y = first_transformed_boundaries.lbn[1] - second_transformed_boundaries.ltn[1];
    }

    if (coll_res.is_near) {
        coll_res.offset_z = second_transformed_boundaries.ltn[2] - first_transformed_boundaries.ltf[2];
    } else {
        coll_res.offset_z = first_transformed_boundaries.ltn[2] - second_transformed_boundaries.ltf[2];
    }

    coll_res.status = coll_res.offset_x <= 0.0f && coll_res.offset_y <= 0.0f && coll_res.offset_z <= 0.0f;

    return coll_res;
}

void my_gl::GeometryObjectPrimitive::handle_collision(my_gl::GeometryObjectPrimitive& second, CollisionResult& coll_res) {
    if (this->_collision_type == CollisionType::GHOST || second._collision_type == CollisionType::GHOST) {
        return;
    }

    my_gl::math::Vec3<float> change_collision_vec(1.0f);

    if (this->_collision_type != CollisionType::STATIC) {
        if (coll_res.offset_x < -0.1f) {
            change_collision_vec[1] = -1.0f;
        }
        if (coll_res.offset_y < -0.1f) {
            change_collision_vec[0] = -1.0f;
        }
        this->_physics->_velocity *= change_collision_vec * second._physics->_mass / this->_physics->_mass;
        // this->_physics->_acceleration *= -1.0f;
    }
    if (second._collision_type != CollisionType::STATIC) {
        if (coll_res.offset_x < -0.1f) {
            change_collision_vec[1] = -1.0f;
        }
        if (coll_res.offset_y < -0.1f) {
            change_collision_vec[0] = -1.0f;
        }
        second._physics->_velocity *= change_collision_vec * this->_physics->_mass / second._physics->_mass;;
        // second._physics->_acceleration *= -1.0f;
    }
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
