#pragma once
#include <GL/glew.h>
#include <cstddef>
#include <vector>
#include "animation.hpp"
#include "matrix.hpp"
#include "texture.hpp"
#include "sharedTypes.hpp"

namespace my_gl {
    class Program;
    class VertexArray;
    class ObjectCache;

    struct TransformsByType {
        TransformsByType(
            math::TransformationType                        arg_type,
            std::vector<math::Transformation<float>>&&      arg_transforms,
            std::vector<my_gl::Animation<float>>&&          arg_anims
        );
        TransformsByType(TransformsByType&& rhs) = default;
        TransformsByType& operator=(TransformsByType&& rhs) = default;
        TransformsByType(const TransformsByType& rhs) = default;
        TransformsByType& operator=(const TransformsByType& rhs) = default;

        math::TransformationType                            type;
        std::vector<math::Transformation<float>>            transforms;
        std::vector<my_gl::Animation<float>>                anims;
    };

    class GeometryObjectPrimitive {
    public:
        GeometryObjectPrimitive(
            std::vector<TransformsByType>&&                     transforms,
            std::size_t                                         vertices_count,
            std::size_t                                         buffer_byte_offset,
            const Program&                                      program,
            const VertexArray&                                  vao,
            GLenum                                              draw_type,
            std::vector<const my_gl::Texture*>&&                textures
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

        constexpr std::size_t       get_vertices_count() const {
            return _vertices_count;
        }
        constexpr std::size_t       get_buffer_byte_offset() const {
            return _buffer_byte_offset;
        }
        const Program&              get_program() const { return _program; }
        const VertexArray&          get_vao() const { return _vao; }

    private:
        std::vector<TransformsByType>                       _transforms;
        std::vector<const my_gl::Texture*>                  _textures;
        std::size_t                                         _vertices_count;
        std::size_t                                         _buffer_byte_offset;
        const Program&                                      _program;
        const VertexArray&                                  _vao;
        GLenum                                              _draw_type;
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
