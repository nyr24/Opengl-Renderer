#include "animation.hpp"
#include "geometryObject.hpp"
#include "matrix.hpp"
#include "renderer.hpp"
#include "vec.hpp"
#include <concepts>
#include <array>
#include <span>

namespace my_gl {
    namespace math {
        template<std::floating_point T>
        class Matrix44;
    }

    // CubeCreature
    constexpr int cube_creature_vert_count{ 36 };

    std::array<my_gl::TransformGroup, 3> transforms = {
        // head
        my_gl::TransformGroup{
            math::TransformationType::TRANSLATION,
            {
                math::Matrix44<float>::translation({0.0f, 1.3f, 0.0f})
            },
            {}
        },
        my_gl::TransformGroup{
            math::TransformationType::ROTATION,
            {
                math::Matrix44<float>::rotation(180.0f, math::Global::AXIS::Y)
            },
            {}
        },
        my_gl::TransformGroup{
            math::TransformationType::SCALING,
            {
                math::Matrix44<float>::scaling({0.8f, 0.5f, 0.4f})
            },
            {}
        }
        // head ends
        // TODO: put other transforms here
    };

    GeometryObjectComplex create_cube_creature(const Program& program, const VertexArray& vertex_array) {
        return std::vector<GeometryObjectPrimitive>{
            // head
            GeometryObjectPrimitive{
                std::span{transforms.begin(), 3},
                Velocity<float>{ {} },
                cube_creature_vert_count,
                0,
                program,
                vertex_array,
                GL_TRIANGLES,
                my_gl::Material::EMERALD,
                nullptr
            },
            // body
            // GeometryObjectPrimitive{
            //     std::vector<TransformGroup>{
            //         {
            //             math::TransformationType::ROTATION,
            //             {
            //                 math::Matrix44<float>::rotation(270.0f, math::Global::AXIS::Y)
            //             },
            //             {}
            //         },
            //         {
            //             math::TransformationType::SCALING,
            //             {
            //                 math::Matrix44<float>::scaling({1.0f, 2.25f, 1.0f})
            //             },
            //             {}
            //         },
            //     },
            //     cube_creature_vert_count,
            //      {},
            //     0,
            //     program,
            //     vertex_array,
            //     GL_TRIANGLES,
            //     my_gl::Material::EMERALD,
            //     nullptr
            // },
            // // left arm
            // GeometryObjectPrimitive{
            //     std::vector<TransformGroup>{
            //         {
            //             math::TransformationType::TRANSLATION,
            //             {
            //                 math::Matrix44<float>::translation({-0.8f, 0.3f, 0.0f})
            //             },
            //             {}
            //         },
            //         {
            //             math::TransformationType::ROTATION,
            //             {
            //                 math::Matrix44<float>::rotation(-55.0f, math::Global::AXIS::Z)
            //             },
            //             {}
            //         },
            //         {
            //             math::TransformationType::SCALING,
            //             {
            //                 math::Matrix44<float>::scaling({0.3f, 1.5f, 0.3f})
            //             },
            //             {}
            //         },
            //     },
            //     cube_creature_vert_count,
            //     0,
            //     program,
            //     vertex_array,
            //     GL_TRIANGLES,
            //     my_gl::Material::EMERALD,
            //     nullptr
            // },
            // // right arm
            // GeometryObjectPrimitive{
            //     std::vector<TransformGroup>{
            //         {
            //             math::TransformationType::TRANSLATION,
            //             {
            //                 math::Matrix44<float>::translation({0.8f, 0.3f, 0.0f})
            //             },
            //             {}
            //         },
            //         {
            //             math::TransformationType::ROTATION3d,
            //             {
            //                 math::Matrix44<float>::rotation3d({0.0f, 0.0f, 55.0f})
            //             },
            //             {
            //                 Animation<float>::rotation3d(
            //                     3.0f,
            //                     5.0f,
            //                     { 0.0f, 0.0f, 0.0f },
            //                     { -90.0f, -45.0f, 0.0f },
            //                     Bezier_curve_type::EASE_IN_OUT,
            //                     Loop_type::INVERT
            //                 )
            //             }
            //         },
            //         {
            //             math::TransformationType::SCALING,
            //             {
            //                 math::Matrix44<float>::scaling({0.3f, 1.5f, 0.3f})
            //             },
            //             {}
            //         },
            //     },
            //     cube_creature_vert_count,
            //     0,
            //     program,
            //     vertex_array,
            //     GL_TRIANGLES,
            //     my_gl::Material::EMERALD,
            //     nullptr
            // },
            // // left foot
            // GeometryObjectPrimitive{
            //     std::vector<TransformGroup>{
            //         {
            //             math::TransformationType::TRANSLATION,
            //             {
            //                 math::Matrix44<float>::translation({-0.25f, -1.5f, 0.0f})
            //             },
            //             {}
            //         },
            //         {
            //             math::TransformationType::ROTATION,
            //             {
            //                 math::Matrix44<float>::rotation(-20.0f, math::Global::AXIS::Z)
            //             },
            //             {}
            //         },
            //         {
            //             math::TransformationType::SCALING,
            //             {
            //                 math::Matrix44<float>::scaling({0.5f, 2.7f, 0.5f})
            //             },
            //             {}
            //         },
            //     },
            //     cube_creature_vert_count,
            //     0,
            //     program,
            //     vertex_array,
            //     GL_TRIANGLES,
            //     my_gl::Material::EMERALD,
            //     nullptr
            // },
            // // right foot
            // GeometryObjectPrimitive{
            //     std::vector<TransformGroup>{
            //         {
            //             math::TransformationType::TRANSLATION,
            //             {
            //                 math::Matrix44<float>::translation({0.25f, -1.5f, 0.0f})
            //             },
            //             {}
            //         },
            //         {
            //             math::TransformationType::ROTATION,
            //             {
            //                 math::Matrix44<float>::rotation(20.0f, math::Global::AXIS::Z)
            //             },
            //             {}
            //         },
            //         {
            //             math::TransformationType::SCALING,
            //             {
            //                 math::Matrix44<float>::scaling({0.5f, 2.7f, 0.5f})
            //             },
            //             {}
            //         },
            //     },
            //     cube_creature_vert_count,
            //     0,
            //     program,
            //     vertex_array,
            //     GL_TRIANGLES,
            //     my_gl::Material::EMERALD,
            //     nullptr
            // }
        };
    }
}
