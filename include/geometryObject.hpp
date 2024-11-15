#pragma once
#include <vector>
#include <GLEW/glew.h>
#include "animation.hpp"
#include "texture.hpp"
#include "sharedTypes.hpp"

namespace my_gl {
    class Program;
    class VertexArray;

    class GeometryObject {
    public:
        GeometryObject(
            std::vector<my_gl_math::Matrix44<float>>&& matrices,
            std::vector<my_gl::Animation<float>>&&     animations,
            std::size_t                                vertices_count,
            std::size_t                                buffer_byte_offset,
            const Program&                             program,
            const VertexArray&                         vao,
            GLenum                                     draw_type,
            const my_gl::Texture*                      texture = nullptr
        );
        GeometryObject(
            std::vector<my_gl::Animation<float>>&&     animations,
            std::size_t                                vertices_count,
            std::size_t                                buffer_byte_offset,
            const Program&                             program,
            const VertexArray&                         vao,
            GLenum                                     draw_type,
            const my_gl::Texture*                      texture = nullptr
        );
        GeometryObject(
            std::vector<my_gl_math::Matrix44<float>>&& matrices,
            std::size_t                                vertices_count,
            std::size_t                                buffer_byte_offset,  
            const Program&                             program,
            const VertexArray&                         vao,
            GLenum                                     draw_type,
            const my_gl::Texture*                      texture = nullptr
        );

        GeometryObject(const GeometryObject& rhs) = default;
        GeometryObject(GeometryObject&& rhs) = default;
        GeometryObject& operator=(const GeometryObject& rhs) = delete;
        GeometryObject& operator=(GeometryObject&& rhs) = delete;
        ~GeometryObject() = default;

        my_gl_math::Matrix44<float> get_local_mat() const;
        void update_anims_time(Duration_sec frame_time) const;
        void bind_state() const;
        void un_bind_state() const;
        void draw() const;

        constexpr std::size_t get_vertices_count() const { 
            return _vertices_count;
        }
        constexpr std::size_t get_buffer_byte_offset() const {
            return _buffer_byte_offset;
        }
        const Program& get_program() const { return _program; }
        const VertexArray& get_vao() const { return _vao; }

    private:
        std::vector<my_gl_math::Matrix44<float>>          _matrices;
        mutable std::vector<my_gl::Animation<float>>      _animations;
        std::size_t                                       _vertices_count;
        std::size_t                                       _buffer_byte_offset;
        const Program&                                    _program;
        const VertexArray&                                _vao;
        const my_gl::Texture*                             _texture{ nullptr };
        GLenum                                            _draw_type;
    };
}