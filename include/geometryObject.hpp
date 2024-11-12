#pragma once
#include <vector>
#include "animation.hpp"
#include "shared_types.hpp"

namespace my_gl {
    class IGeometry_object {
    public:
        virtual my_gl_math::Matrix44<float> get_local_mat() const = 0;
        virtual void update_anims_time(Duration_sec frame_time) const = 0; 
        virtual constexpr std::size_t get_vertices_count() const = 0;
        virtual constexpr std::size_t get_buffer_byte_offset() const = 0;

    private:
        std::vector<my_gl_math::Matrix44<float>>  _matrices;
        std::vector<my_gl::Animation<float>>      _animations;
        std::size_t                               _vertices_count;
        std::size_t                               _buffer_byte_offset;
    };

    class Cube : public IGeometry_object {
    public:
        static constexpr std::size_t CUBE_VERT_COUNT = 24;

        Cube(
            std::vector<my_gl_math::Matrix44<float>>&& matrices,
            std::vector<my_gl::Animation<float>>&&     animations,
            std::size_t                                vertices_count,
            std::size_t                                buffer_byte_offset   
        );
        Cube(
            std::vector<my_gl::Animation<float>>&&     animations,
            std::size_t                                vertices_count,
            std::size_t                                buffer_byte_offset   
        );
        Cube(
            std::vector<my_gl_math::Matrix44<float>>&& matrices,
            std::size_t                                vertices_count,
            std::size_t                                buffer_byte_offset   
        );
        Cube(const Cube& rhs) = default;
        Cube(Cube&& rhs) = default;
        Cube& operator=(const Cube& rhs) = default;
        Cube& operator=(Cube&& rhs) = default;
        ~Cube() = default;

        my_gl_math::Matrix44<float> get_local_mat() const override;
        void update_anims_time(Duration_sec frame_time) const override;
        
        constexpr std::size_t get_vertices_count() const override { 
            return _vertices_count;
        }
        constexpr std::size_t get_buffer_byte_offset() const override {
            return _buffer_byte_offset;
        }

    private:
        std::vector<my_gl_math::Matrix44<float>>          _matrices;
        mutable std::vector<my_gl::Animation<float>>      _animations;
        std::size_t                                       _vertices_count;
        std::size_t                                       _buffer_byte_offset;
    };
}