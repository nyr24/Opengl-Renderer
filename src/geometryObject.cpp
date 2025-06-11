#include "geometryObject.hpp"
#include "advanced_transforms.hpp"
#include "matrix.hpp"
#include "meshes.hpp"
#include "renderer.hpp"
#include "sharedTypes.hpp"
#include "appCode.hpp"
#include "vec.hpp"
#include <cstdio>
#include <span>
#include <utility>
#ifdef DEBUG
#include <iostream>
#endif

my_gl::TransformData::TransformData(
    math::TransformationType                arg_type,
    math::Matrix44<float>&&                 arg_static_transform,
    Option<my_gl::TimeTransform<float>>&&   arg_time_transform,
    Option<my_gl::FrameTransform<float>>&&  arg_frame_transform
)
    : type{ arg_type }
    , static_transform{ std::move(arg_static_transform) }
    , time_transform{ std::move(arg_time_transform) }
    , frame_transform{ std::move(arg_frame_transform) }
{}

my_gl::GeometryObjectPrimitive::GeometryObjectPrimitive(
    std::span<TransformData>        transform_data,
    void* const                     custom_obj_state,
    FrameTransform<float>* const    movement,
    const Texture* const            texture,
    const Program&                  program,
    const VertexArray&              vao,
    std::size_t                     vertices_count,
    std::size_t                     buffer_byte_offset,
    GLenum                          draw_type,
    Material::Type                  material_type,
    CollisionType                   collision_type
)
    : _transform_data{ transform_data }
    , _custom_obj_state{ custom_obj_state }
    , _movement{ movement }
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

void my_gl::GeometryObjectPrimitive::calc_model_mat_frame(Duration_sec delta_time) {
    auto result_mat{ my_gl::math::Matrix44<float>::identity_new() };

    if (_transform_data.size()) {
        for (my_gl::TransformData& transforms_by_type : _transform_data) {
            if (transforms_by_type.type == math::TransformationType::TRANSLATION && _collision_type != CollisionType::STATIC && _movement) {
                result_mat *= transforms_by_type.static_transform;
                result_mat *= _movement->update(delta_time.count());
                if (auto* time_transform = transforms_by_type.time_transform.get_if()) {
                    result_mat *= *(time_transform->update(delta_time).get_mat());
                }
                if (auto* frame_transform = transforms_by_type.frame_transform.get_if()) {
                    result_mat *= frame_transform->update(delta_time.count());
                }
            } else {
                result_mat *= transforms_by_type.static_transform;
                if (auto* time_transform = transforms_by_type.time_transform.get_if()) {
                    result_mat *= *(time_transform->update(delta_time).get_mat());
                }
                if (auto* frame_transform = transforms_by_type.frame_transform.get_if()) {
                    result_mat *= frame_transform->update(delta_time.count());
                }
            }
        }
    }

    _model_mat = std::move(result_mat);
}

// void my_gl::GeometryObjectPrimitive::update_anims_time(Duration_sec delta_time) {
//     for (auto& transform_by_type : _transform_data) {
//         if (auto* time_t = transform_by_type.time_transform.get_if()) {
//             time_t->update_time(delta_time);
//         }
//     }
// }

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
    Duration_sec delta_time,
    float time_0to1
)
{
    bind_state();

    this->calc_model_mat_frame(delta_time);
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

    if (_custom_obj_state) {
        // auto* custom_obj_state = static_cast<GameObjState*>(_custom_obj_state);
        // if (custom_obj_state->type == GameObjType::TILE && !custom_obj_state->alive) {
        //     _program.set_uniform_value("u_opacity", *(custom_obj_state->opacity.update().get_scalar()));
        //     custom_obj_state->opacity.update_time(delta_time);
        // }
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
    assert(coll_res.status && "Collision should happend, before calling this function");
    if (this->_collision_type == CollisionType::GHOST || second._collision_type == CollisionType::GHOST) {
        return;
    }

    // NOTE: temp code, remove later
    GameObjState* obj_state1 = static_cast<GameObjState*>(this->_custom_obj_state);
    GameObjState* obj_state2 = static_cast<GameObjState*>(second._custom_obj_state);
    printf("Collision between %i : %i\n", obj_state1->type, obj_state2->type);

    // if (this->_custom_obj_state) {
    //     GameObjState* obj_state = static_cast<GameObjState*>(this->_custom_obj_state);
    //     if (obj_state->type == GameObjType::TILE && obj_state->alive) {
    //         this->_collision_type = CollisionType::GHOST;
    //         obj_state->alive = false;
    //         obj_state->opacity._is_paused = false;
    //         printf("obj is dead: type = %i\n", obj_state->type);
    //     }
    // }
    // if (second._custom_obj_state) {
    //     GameObjState* obj_state = static_cast<GameObjState*>(second._custom_obj_state);
    //     if (obj_state->type == GameObjType::TILE && obj_state->alive) {
    //         this->_collision_type = CollisionType::GHOST;
    //         obj_state->alive = false;
    //         obj_state->opacity._is_paused = false;
    //         printf("obj is dead: type = %i\n", obj_state->type);
    //     }
    // }

    my_gl::math::Vec3<float> change_collision_vec(1.0f);

    if (this->_collision_type != CollisionType::STATIC) {
        if (coll_res.offset_x < -0.05f) {
            change_collision_vec[1] = -1.0f;
        }
        if (coll_res.offset_y < -0.05f) {
            change_collision_vec[0] = -1.0f;
        }
        this->_movement->_velocity *= change_collision_vec * second._mass / this->_mass;
        // this->_movement->_acceleration *= -1.0f;
    }
    if (second._collision_type != CollisionType::STATIC) {
        if (coll_res.offset_x < -0.05f) {
            change_collision_vec[1] = -1.0f;
        }
        if (coll_res.offset_y < -0.05f) {
            change_collision_vec[0] = -1.0f;
        }
        second._movement->_velocity *= change_collision_vec * this->_mass / second._mass;
        // second._movement->_acceleration *= -1.0f;
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
    Duration_sec delta_time,
    float time_0to1
)
{
    for (auto& primitive : _primitives) {
        primitive.render(view_mat, view_proj_mat, delta_time, time_0to1);
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
