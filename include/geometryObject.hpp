#pragma once
#include <GL/glew.h>
#include <cstddef>
#include <vector>
#include <array>
#include <span>
#include "animation.hpp"
#include "matrix.hpp"
#include "texture.hpp"
#include "sharedTypes.hpp"

namespace my_gl {
    class Program;
    class VertexArray;
    class ObjectCache;

    struct TransformGroup {
        TransformGroup(
            math::TransformationType                        arg_type,
            std::vector<math::Transformation<float>>&&      arg_transforms,
            std::vector<my_gl::Animation<float>>&&          arg_anims
        );
        TransformGroup(TransformGroup&& rhs) = default;
        TransformGroup& operator=(TransformGroup&& rhs) = default;
        TransformGroup(const TransformGroup& rhs) = default;
        TransformGroup& operator=(const TransformGroup& rhs) = default;

        math::TransformationType                            type;
        std::vector<math::Transformation<float>>            transforms;
        std::vector<my_gl::Animation<float>>                anims;
    };

    struct Material {
        // this enum is also an index for material_data table
        // order is important
        enum Type : uint16_t {
            EMERALD,
            OBSIDIAN,
            RUBY,
            TURQUOISE,
            GOLD,
            CYAN_PLASTIC,
            GREEN_RUBBER,

            COUNT,
            NO_MATERIAL
        };

        my_gl::math::Vec3<float>    ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
        my_gl::math::Vec3<float>    diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
        my_gl::math::Vec3<float>    specular = { 1.0f, 1.0f, 1.0f, 1.0f };
        float                       shininess = 32.0f;

        Material(
            my_gl::math::Vec3<float>&&      ambient_,
            my_gl::math::Vec3<float>&&      diffuse_,
            my_gl::math::Vec3<float>&&      specular_,
            float                           shininess_
        );
        Material(const Material& rhs) = default;
        Material(Material&& rhs) = default;
        Material& operator=(const Material& rhs) = default;
        Material& operator=(Material&& rhs) = default;

        static const Material& get_from_table(Type material_type);
    };

    extern const std::array<Material, Material::COUNT> material_table;

    class GeometryObjectPrimitive {
    public:
        GeometryObjectPrimitive(
            std::span<my_gl::TransformGroup>                    transforms,
            std::size_t                                         vertices_count,
            std::size_t                                         buffer_byte_offset,
            const Program&                                      program,
            const VertexArray&                                  vao,
            GLenum                                              draw_type,
            Material::Type                                      material_type,
            const Texture* const                                texture
        );

        GeometryObjectPrimitive(GeometryObjectPrimitive&& rhs) = default;
        GeometryObjectPrimitive(const GeometryObjectPrimitive& rhs) = default;
        ~GeometryObjectPrimitive() = default;

        math::Matrix44<float>       get_model_mat();
        void                        bind_state() const;
        void                        un_bind_state() const;
        void                        draw() const;
        void                        update_anims_time(Duration_sec frame_time);
        void                        render(const math::Matrix44<float>& view_mat, const math::Matrix44<float>& view_proj_mat, float time_0to1);

        std::span<TransformGroup>   _transforms;
        const Texture* const        _texture;
        const Program&              _program;
        const VertexArray&          _vao;
        std::size_t                 _vertices_count;
        std::size_t                 _buffer_byte_offset;
        GLenum                      _draw_type;
        Material::Type              _material_type;
    };

    class GeometryObjectComplex {
    public:
        GeometryObjectComplex(std::vector<GeometryObjectPrimitive>&& primitives);
        GeometryObjectComplex(const std::vector<GeometryObjectPrimitive>& primitives);

        void render(const math::Matrix44<float>& view_mat, const math::Matrix44<float>& view_proj_mat, float time_0to1);
        void update_anims_time(Duration_sec frame_time);
    private:
        std::vector<GeometryObjectPrimitive> _primitives;
    };
}
