#include "cubeCreature.hpp"
#include "geometryObject.hpp"
#include "math.hpp"
#include "matrix.hpp"

namespace my_gl {
    CubeCreature::CubeCreature(
        GeometryObject&& head,
        GeometryObject&& body,
        GeometryObject&& left_hand,
        GeometryObject&& right_hand,
        GeometryObject&& left_leg,
        GeometryObject&& right_leg
    )
        : _head{ std::move(head) }
        , _body{ std::move(body) }
        , _left_hand{ std::move(left_hand) }
        , _right_hand{ std::move(right_hand) }
        , _left_leg{ std::move(left_leg) }
        , _right_leg{ std::move(right_leg) }
    {}

    CubeCreature init(const Program& program, const VertexArray& vertex_array) {
        constexpr int vert_count{36};

        // components of resulting model
        my_gl::GeometryObject head{
            {
                my_gl_math::Matrix44<float>::scaling({1.25f, 0.75f, 1.0f}),
                my_gl_math::Matrix44<float>::translation({0.0f, 0.5f, 0.0f}),
            },
            vert_count,
            vert_count * 0,
            program,
            vertex_array,
            GL_TRIANGLES,
            {}
        };

        my_gl::GeometryObject body{
            {
                my_gl_math::Matrix44<float>::scaling({0.6f, 3.5f, 0.6f}),
            },
            vert_count,
            vert_count * 1,
            program,
            vertex_array,
            GL_TRIANGLES,
            {}
        };

        my_gl::GeometryObject left_hand{
            {
                my_gl_math::Matrix44<float>::scaling({0.3f, 1.3f, 0.6f}),
                my_gl_math::Matrix44<float>::rotation(-35.0f, my_gl_math::Global::AXIS::Z),
                my_gl_math::Matrix44<float>::translation({-0.5f, 0.35f, 0.0f}),
            },
            vert_count,
            vert_count * 2,
            program,
            vertex_array,
            GL_TRIANGLES,
            {}
        };

        my_gl::GeometryObject right_hand{
            {
                my_gl_math::Matrix44<float>::scaling({0.3f, 1.3f, 0.6f}),
                my_gl_math::Matrix44<float>::rotation(35.0f, my_gl_math::Global::AXIS::Z),
                my_gl_math::Matrix44<float>::translation({0.5f, 0.35f, 0.0f}),
            },
            vert_count,
            vert_count * 3,
            program,
            vertex_array,
            GL_TRIANGLES,
            {}
        };

        my_gl::GeometryObject left_leg{
            {
                my_gl_math::Matrix44<float>::scaling({0.5f, 2.5f, 0.6f}),
                my_gl_math::Matrix44<float>::rotation(-20.0f, my_gl_math::Global::AXIS::Z),
                my_gl_math::Matrix44<float>::translation({-0.25f, -1.25f, 0.0f}),
            },
            vert_count,
            vert_count * 4,
            program,
            vertex_array,
            GL_TRIANGLES,
            {}
        };

        my_gl::GeometryObject right_leg{
            {
                my_gl_math::Matrix44<float>::scaling({0.5f, 2.5f, 0.6f}),
                my_gl_math::Matrix44<float>::rotation(20.0f, my_gl_math::Global::AXIS::Z),
                my_gl_math::Matrix44<float>::translation({0.25f, -1.25f, 0.0f}),
            },
            vert_count,
            vert_count * 5,
            program,
            vertex_array,
            GL_TRIANGLES,
            {}
        };

        return CubeCreature{
            std::move(head),
            std::move(body),
            std::move(left_hand),
            std::move(right_hand),
            std::move(left_leg),
            std::move(right_leg)
        };
    }
}
