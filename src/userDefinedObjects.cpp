#include "animation.hpp"
#include "geometryObject.hpp"
#include "renderer.hpp"
#include "vec.hpp"
#include <vector>

namespace my_gl {
    // CubeCreature
    constexpr int cube_creature_vert_count{ 36 };

    GeometryObjectComplex create_cube_creature(const Program& program, const VertexArray& vertex_array) {
        return std::vector<GeometryObjectPrimitive>{
            // head
            my_gl::GeometryObjectPrimitive{
                {
                    my_gl_math::Matrix44<float>::translation({0.0f, 1.3f, 0.0f}),
                    my_gl_math::Matrix44<float>::rotation(180.0f, my_gl_math::Global::AXIS::Y),
                    my_gl_math::Matrix44<float>::scaling({0.8f, 0.5f, 0.4f}),
                },
                cube_creature_vert_count,
                0,
                program,
                vertex_array,
                GL_TRIANGLES,
                {}
            },
            // body
            my_gl::GeometryObjectPrimitive{
                {
                    my_gl_math::Matrix44<float>::scaling({1.0f, 2.25f, 1.0f}),
                    my_gl_math::Matrix44<float>::rotation(270.0f, my_gl_math::Global::AXIS::Y),
                },
                cube_creature_vert_count,
                0,
                program,
                vertex_array,
                GL_TRIANGLES,
                {}
            },
            // left arm
            my_gl::GeometryObjectPrimitive{
                {
                    my_gl_math::Matrix44<float>::translation({-0.8f, 0.3f, 0.0f}),
                    my_gl_math::Matrix44<float>::rotation(-55.0f, my_gl_math::Global::AXIS::Z),
                    my_gl_math::Matrix44<float>::scaling({0.3f, 1.5f, 0.3f}),
                },
                {
                    {
                        my_gl::Animation_type::ROTATE3d,
                        3.0f,
                        0.0f,
                        { 0.0f, 0.0f, 0.0f },
                        { -90.0f, -45.0f, 0.0f },
                        my_gl::Bezier_curve_type::EASE_IN_OUT,
                        my_gl::Loop_type::INVERT
                    }
                },
                cube_creature_vert_count,
                0,
                program,
                vertex_array,
                GL_TRIANGLES,
                {}
            },
            // right arm
            my_gl::GeometryObjectPrimitive{
                {
                    my_gl_math::Matrix44<float>::translation({0.8f, 0.3f, 0.0f}),
                    my_gl_math::Matrix44<float>::rotation(55.0f, my_gl_math::Global::AXIS::Z),
                    my_gl_math::Matrix44<float>::scaling({0.3f, 1.5f, 0.3f}),
                },
                {
                    {
                        my_gl::Animation_type::ROTATE3d,
                        3.0f,
                        3.0f,
                        { 0.0f, 0.0f, 0.0f },
                        { -90.0f, -45.0f, 0.0f },
                        my_gl::Bezier_curve_type::EASE_IN_OUT,
                        my_gl::Loop_type::INVERT
                    }
                },
                cube_creature_vert_count,
                0,
                program,
                vertex_array,
                GL_TRIANGLES,
                {}
            },
            // left foot
            my_gl::GeometryObjectPrimitive{
                {
                    my_gl_math::Matrix44<float>::translation({-0.25f, -1.5f, 0.0f}),
                    my_gl_math::Matrix44<float>::rotation(-20.0f, my_gl_math::Global::AXIS::Z),
                    my_gl_math::Matrix44<float>::scaling({0.5f, 2.7f, 0.5f}),
                },
                cube_creature_vert_count,
                0,
                program,
                vertex_array,
                GL_TRIANGLES,
                {}
            },
            // right foot
            my_gl::GeometryObjectPrimitive{
                {
                    my_gl_math::Matrix44<float>::translation({0.25f, -1.5f, 0.0f}),
                    my_gl_math::Matrix44<float>::rotation(20.0f, my_gl_math::Global::AXIS::Z),
                    my_gl_math::Matrix44<float>::scaling({0.5f, 2.7f, 0.5f}),
                },
                cube_creature_vert_count,
                0,
                program,
                vertex_array,
                GL_TRIANGLES,
                {}
            }
        };
    }
}
