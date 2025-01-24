#include "cubeCreature.hpp"
#include "geometryObject.hpp"
#include "math.hpp"
#include "matrix.hpp"
#include "sharedTypes.hpp"

namespace my_gl {
    CubeCreature::CubeCreature(CubeCreatureParams&& params)
        : _head{ std::move(params.head) }
        , _body{ std::move(params.body) }
        , _left_hand{ std::move(params.left_hand) }
        , _right_hand{ std::move(params.right_hand) }
        , _left_leg{ std::move(params.left_leg) }
        , _right_leg{ std::move(params.right_leg) }
    {}

    CubeCreature CubeCreature::create(const Program& program, const VertexArray& vertex_array) {
        constexpr int vert_count{36};

        // components of resulting model
        my_gl::GeometryObject head{
            {
                my_gl_math::Matrix44<float>::scaling({1.25f, 0.4f, 0.4f}),
                my_gl_math::Matrix44<float>::rotation(180.0f, my_gl_math::Global::AXIS::Y),
                my_gl_math::Matrix44<float>::translation({0.0f, 3.5f, 0.0f}),
            },
            vert_count,
            0,
            program,
            vertex_array,
            GL_TRIANGLES,
            {}
        };

        my_gl::GeometryObject body{
            {
                my_gl_math::Matrix44<float>::scaling({0.7f, 2.5f, 0.7f}),
                my_gl_math::Matrix44<float>::rotation(270.0f, my_gl_math::Global::AXIS::Y),
            },
            vert_count,
            0,
            program,
            vertex_array,
            GL_TRIANGLES,
            {}
        };

        my_gl::GeometryObject left_hand{
            {
                my_gl_math::Matrix44<float>::scaling({0.3f, 1.3f, 0.3f}),
                my_gl_math::Matrix44<float>::rotation(-45.0f, my_gl_math::Global::AXIS::Z),
                my_gl_math::Matrix44<float>::translation({-2.5f, 0.15f, 0.0f}),
            },
            vert_count,
            0,
            program,
            vertex_array,
            GL_TRIANGLES,
            {}
        };

        my_gl::GeometryObject right_hand{
            {
                my_gl_math::Matrix44<float>::scaling({0.3f, 1.3f, 0.3f}),
                my_gl_math::Matrix44<float>::rotation(45.0f, my_gl_math::Global::AXIS::Z),
                my_gl_math::Matrix44<float>::translation({2.5f, 0.15f, 0.0f}),
            },
            vert_count,
            0,
            program,
            vertex_array,
            GL_TRIANGLES,
            {}
        };

        my_gl::GeometryObject left_leg{
            {
                my_gl_math::Matrix44<float>::scaling({0.5f, 2.5f, 0.5f}),
                my_gl_math::Matrix44<float>::rotation(-25.0f, my_gl_math::Global::AXIS::Z),
                my_gl_math::Matrix44<float>::translation({-0.25f, -1.25f, 0.0f}),
            },
            vert_count,
            0,
            program,
            vertex_array,
            GL_TRIANGLES,
            {}
        };

        my_gl::GeometryObject right_leg{
            {
                my_gl_math::Matrix44<float>::scaling({0.5f, 2.5f, 0.5f}),
                my_gl_math::Matrix44<float>::rotation(25.0f, my_gl_math::Global::AXIS::Z),
                my_gl_math::Matrix44<float>::translation({0.25f, -1.25f, 0.0f}),
            },
            vert_count,
            0,
            program,
            vertex_array,
            GL_TRIANGLES,
            {}
        };

        return CubeCreature(
            {
                .head = std::move(head),
                .body = std::move(body),
                .left_hand = std::move(left_hand),
                .right_hand = std::move(right_hand),
                .left_leg = std::move(left_leg),
                .right_leg = std::move(right_leg)
            }
        );
    }

    void CubeCreature::render(const my_gl_math::Matrix44<float>& world_matrix, float time_0to1) const {
        _head.render(world_matrix, time_0to1);
        _body.render(world_matrix, time_0to1);
        _left_hand.render(world_matrix, time_0to1);
        _right_hand.render(world_matrix, time_0to1);
        _left_leg.render(world_matrix, time_0to1);
        _right_leg.render(world_matrix, time_0to1);
    }

    void CubeCreature::update_anims_time(Duration_sec frame_time) const {
        _head.update_anims_time(frame_time);
        _body.update_anims_time(frame_time);
        _left_hand.update_anims_time(frame_time);
        _right_hand.update_anims_time(frame_time);
        _left_leg.update_anims_time(frame_time);
        _right_leg.update_anims_time(frame_time);
    }
}
