#pragma once
#include <vector>
#include "animation.hpp"
#include "sharedTypes.hpp"

namespace my_gl {
    class Geometry_object {
    public:
        Geometry_object(
            std::vector<my_gl_math::Matrix44<float>>&& matrices,
            std::vector<my_gl::Animation<float>>&&     animations,
            std::size_t                                vertices_count,
            std::size_t                                buffer_byte_offset   
        );
        Geometry_object(
            std::vector<my_gl::Animation<float>>&&     animations,
            std::size_t                                vertices_count,
            std::size_t                                buffer_byte_offset   
        );
        Geometry_object(
            std::vector<my_gl_math::Matrix44<float>>&& matrices,
            std::size_t                                vertices_count,
            std::size_t                                buffer_byte_offset   
        );

        Geometry_object(const Geometry_object& rhs) = default;
        Geometry_object(Geometry_object&& rhs) = default;
        Geometry_object& operator=(const Geometry_object& rhs) = default;
        Geometry_object& operator=(Geometry_object&& rhs) = default;
        ~Geometry_object() = default;

        my_gl_math::Matrix44<float> get_local_mat() const;
        void update_anims_time(Duration_sec frame_time) const;
        
        constexpr std::size_t get_vertices_count() const { 
            return _vertices_count;
        }
        constexpr std::size_t get_buffer_byte_offset() const {
            return _buffer_byte_offset;
        }

    private:
        std::vector<my_gl_math::Matrix44<float>>          _matrices;
        mutable std::vector<my_gl::Animation<float>>      _animations;
        std::size_t                                       _vertices_count;
        std::size_t                                       _buffer_byte_offset;
    };
}