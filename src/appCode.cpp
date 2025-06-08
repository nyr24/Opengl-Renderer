#include "appCode.hpp"
#include "geometryObject.hpp"
#include "matrix.hpp"
#include <GLFW/glfw3.h>
#include <sys/types.h>
#include <vector>
#include <span>
#ifdef DEBUG
#include <iostream>
#endif

void process_keyboard_input(GameState* game_state, int32_t key_pressed) {
    if (!game_state) {
        std::cerr << "No game state found\n";
        return;
    }

    switch (key_pressed) {
        case GLFW_KEY_LEFT: {
            my_gl::CollisionResult left_wall_coll_res = game_state->primitives[PrimitiveIndex::PADDLE].check_collision(game_state->primitives[PrimitiveIndex::LEFT_WALL]);
            if (left_wall_coll_res.status) {
                return;
            }
            my_gl::CollisionResult ball_coll_res = game_state->primitives[PrimitiveIndex::PADDLE].check_collision(game_state->primitives[PrimitiveIndex::BALL]);
            if (ball_coll_res.status) {
                return;
            }

            float move_offset = std::min(left_wall_coll_res.offset_x, PADDLE_MOVE_STEP_X);
            if (!ball_coll_res.is_left && ball_coll_res.offset_y < 0) {
                move_offset = std::min(move_offset, ball_coll_res.offset_x);
            }
            game_state->transforms[2].transforms[0].translate_acc({ -(move_offset - 0.01f), 0.0f, 0.0f });
        } break;
        case GLFW_KEY_RIGHT: {
            my_gl::CollisionResult right_wall_coll_res = game_state->primitives[PrimitiveIndex::PADDLE].check_collision(game_state->primitives[PrimitiveIndex::RIGHT_WALL]);
            if (right_wall_coll_res.status) {
                return;
            }
            my_gl::CollisionResult ball_coll_res = game_state->primitives[PrimitiveIndex::PADDLE].check_collision(game_state->primitives[PrimitiveIndex::BALL]);
            if (ball_coll_res.status) {
                return;
            }

            float move_offset = std::min(right_wall_coll_res.offset_x, PADDLE_MOVE_STEP_X);
            if (ball_coll_res.is_left && ball_coll_res.offset_y < 0) {
                move_offset = std::min(move_offset, ball_coll_res.offset_x);
            }
            game_state->transforms[2].transforms[0].translate_acc({ (move_offset - 0.01f), 0.0f, 0.0f });
        } break;
        case GLFW_KEY_UP: {
            my_gl::CollisionResult top_wall_coll_res = game_state->primitives[PrimitiveIndex::PADDLE].check_collision(game_state->primitives[PrimitiveIndex::TOP_WALL]);
            if (top_wall_coll_res.status) {
                return;
            }
            my_gl::CollisionResult ball_coll_res = game_state->primitives[PrimitiveIndex::PADDLE].check_collision(game_state->primitives[PrimitiveIndex::BALL]);
            if (ball_coll_res.status) {
                return;
            }

            float move_offset = std::min(top_wall_coll_res.offset_y, PADDLE_MOVE_STEP_Y);
            if (ball_coll_res.is_bottom && ball_coll_res.offset_x < 0) {
                move_offset = std::min(move_offset, ball_coll_res.offset_y);
            }
            game_state->transforms[2].transforms[0].translate_acc({ 0.0f, (move_offset - 0.01f), 0.0f });
        } break;
        case GLFW_KEY_DOWN: {
            my_gl::CollisionResult bottom_wall_coll_res = game_state->primitives[PrimitiveIndex::PADDLE].check_collision(game_state->primitives[PrimitiveIndex::BOTTOM_WALL]);
            if (bottom_wall_coll_res.status) {
                return;
            }
            my_gl::CollisionResult ball_coll_res = game_state->primitives[PrimitiveIndex::PADDLE].check_collision(game_state->primitives[PrimitiveIndex::BALL]);
            if (ball_coll_res.status) {
                return;
            }

            float move_offset = std::min(bottom_wall_coll_res.offset_y, PADDLE_MOVE_STEP_Y);
            if (!ball_coll_res.is_bottom && ball_coll_res.offset_x < 0) {
                move_offset = std::min(move_offset, ball_coll_res.offset_y);
            }
            game_state->transforms[2].transforms[0].translate_acc({ 0.0f, -(move_offset - 0.01f), 0.0f });
        } break;
    }
}

void populate_primitive_transforms(std::vector<my_gl::TransformData>& out_transforms) {
    assert((out_transforms.capacity() - out_transforms.size()) >= (PrimitiveIndex::COUNT * TRANSFORMS_PER_PRIMITIVE) && "Bad reserve count");

    // ball
    out_transforms.push_back(my_gl::TransformData{
        my_gl::math::TransformationType::TRANSLATION,
        {
            my_gl::math::Matrix44<float>::translation({ 0.0f, BALL_INIT_OFFSET_Y, 0.0f }),
        },
        {}
    });

    out_transforms.push_back(my_gl::TransformData{
        my_gl::math::TransformationType::SCALING,
        {
            my_gl::math::Matrix44<float>::scaling({ 1.3f, 0.6f, 1.0f }),
        },
        {}
    });

    // paddle
    out_transforms.push_back(my_gl::TransformData{
        my_gl::math::TransformationType::TRANSLATION,
        {
            my_gl::math::Matrix44<float>::translation({ 0.0f, -VERT_WALL_OFFSET / 2, 0.0f }),
        },
        {}
    });

    out_transforms.push_back(my_gl::TransformData{
        my_gl::math::TransformationType::SCALING,
        {
            my_gl::math::Matrix44<float>::scaling({ 1.0f, 0.3f, 1.0f }),
        },
        {}
    });

    // left wall
    out_transforms.push_back(my_gl::TransformData{
        my_gl::math::TransformationType::TRANSLATION,
        {
            my_gl::math::Matrix44<float>::translation({ -VERT_WALL_OFFSET, 0.0f, 0.0f })
        },
        {}
    });

    out_transforms.push_back(my_gl::TransformData{
        my_gl::math::TransformationType::SCALING,
        {
            my_gl::math::Matrix44<float>::scaling({ VERT_WALL_WIDTH, VERT_WALL_HEIGHT, 1.0f }),
        },
        {}
    });

    // right wall
    out_transforms.push_back(my_gl::TransformData{
        my_gl::math::TransformationType::TRANSLATION,
        {
            my_gl::math::Matrix44<float>::translation({ VERT_WALL_OFFSET, 0.0f, 0.0f })
        },
        {}
    });

    out_transforms.push_back(my_gl::TransformData{
        my_gl::math::TransformationType::SCALING,
        {
            my_gl::math::Matrix44<float>::scaling({ VERT_WALL_WIDTH, VERT_WALL_HEIGHT, 1.0f }),
        },
        {}
    });

    // top wall
    out_transforms.push_back(my_gl::TransformData{
        my_gl::math::TransformationType::TRANSLATION,
        {
            my_gl::math::Matrix44<float>::translation({ 0.0f, HOR_WALL_OFFSET, 0.0f })
        },
        {}
    });

    out_transforms.push_back(my_gl::TransformData{
        my_gl::math::TransformationType::SCALING,
        {
            my_gl::math::Matrix44<float>::scaling({ HOR_WALL_WIDTH, HOR_WALL_HEIGHT, 1.0f }),
        },
        {}
    });

    // bottom wall
    out_transforms.push_back(my_gl::TransformData{
        my_gl::math::TransformationType::TRANSLATION,
        {
            my_gl::math::Matrix44<float>::translation({ 0.0f, -HOR_WALL_OFFSET, 0.0f })
        },
        {}
    });

    out_transforms.push_back(my_gl::TransformData{
        my_gl::math::TransformationType::SCALING,
        {
            my_gl::math::Matrix44<float>::scaling({ HOR_WALL_WIDTH, HOR_WALL_HEIGHT, 1.0f }),
        },
        {}
    });

    // light
    // out_transforms.push_back(my_gl::TransformData{
    //     my_gl::math::TransformationType::SCALING,
    //     {
    //         my_gl::math::Matrix44<float>::scaling({0.4f, 0.4f, 0.8f}),
    //     },
    //     {}
    // });
    //
    // out_transforms.push_back(my_gl::TransformData{
    //     my_gl::math::TransformationType::TRANSLATION,
    //     {
    //         my_gl::math::Matrix44<float>::translation(my_gl::globals::light.position),
    //     },
    //     {}
    // });
    //
    // out_transforms.push_back(my_gl::TransformData{
    //     my_gl::math::TransformationType::ROTATION,
    //     {
    //         my_gl::math::Matrix44<float>::rotation(-270.0f, my_gl::math::Global::X),
    //     },
    //     {}
    // });
}

void populate_tile_transforms(std::vector<my_gl::TransformData>& out_transforms) {
    assert((out_transforms.capacity() - out_transforms.size()) >= TILE_TRANSFORM_COUNT && "Bad reserve count");

    for (uint16_t i{ 0 }; i < TILE_ROWS; ++i) {
        for (uint16_t j{ 0 }; j < TILE_COLS; ++j) {
            out_transforms.push_back(my_gl::TransformData{
                my_gl::math::TransformationType::TRANSLATION,
                { my_gl::math::Matrix44<float>::translation({TILES_INIT_LEFT_OFFSET + j * TILE_OFFSET_X, TILES_INIT_TOP_OFFSET - i * TILE_OFFSET_Y, 0.0f}) },
                {}
            });

            out_transforms.push_back(my_gl::TransformData{
                my_gl::math::TransformationType::SCALING,
                { my_gl::math::Matrix44<float>::scaling({TILE_WIDTH, TILE_HEIGHT, 1.0f}) },
                {}
            });
        }
    }
}

void populate_primitives(
    std::vector<my_gl::GeometryObjectPrimitive>& out_primitives,
    std::vector<my_gl::TransformData>& transforms,
    std::span<my_gl::Physics<float>> physics,
    const my_gl::Program& shader,
    const my_gl::VertexArray& vert_arr
)
{
    assert((out_primitives.capacity() - out_primitives.size() >= PrimitiveIndex::COUNT) && "Bad reserve count");

    // ball
    out_primitives.push_back(my_gl::GeometryObjectPrimitive{
        std::span<my_gl::TransformData>{transforms.data(), TRANSFORMS_PER_PRIMITIVE},
        &physics[0],
        36,
        0,
        shader,
        vert_arr,
        GL_TRIANGLES,
        my_gl::Material::EMERALD,
        my_gl::CollisionType::DEFAULT,
        nullptr,
    });

    // paddle
    out_primitives.push_back(my_gl::GeometryObjectPrimitive{
        std::span<my_gl::TransformData>{transforms.data() + 2, TRANSFORMS_PER_PRIMITIVE},
        &physics[1],
        36,
        0,
        shader,
        vert_arr,
        GL_TRIANGLES,
        my_gl::Material::RUBY,
        my_gl::CollisionType::STATIC,
        nullptr,
    });

    // left wall
    out_primitives.push_back(my_gl::GeometryObjectPrimitive{
        std::span<my_gl::TransformData>{transforms.data() + 4, TRANSFORMS_PER_PRIMITIVE},
        &physics[1],
        36,
        0,
        shader,
        vert_arr,
        GL_TRIANGLES,
        my_gl::Material::OBSIDIAN,
        my_gl::CollisionType::STATIC,
        nullptr,
    });

    // right wall
    out_primitives.push_back(my_gl::GeometryObjectPrimitive{
        std::span<my_gl::TransformData>{transforms.data() + 6, TRANSFORMS_PER_PRIMITIVE},
        &physics[1],
        36,
        0,
        shader,
        vert_arr,
        GL_TRIANGLES,
        my_gl::Material::OBSIDIAN,
        my_gl::CollisionType::STATIC,
        nullptr,
    });

    // top wall
    out_primitives.push_back(my_gl::GeometryObjectPrimitive{
        std::span<my_gl::TransformData>{transforms.data() + 8, TRANSFORMS_PER_PRIMITIVE},
        &physics[1],
        36,
        0,
        shader,
        vert_arr,
        GL_TRIANGLES,
        my_gl::Material::OBSIDIAN,
        my_gl::CollisionType::STATIC,
        nullptr,
    });

    // bottom wall
    out_primitives.push_back(my_gl::GeometryObjectPrimitive{
        std::span<my_gl::TransformData>{transforms.data() + 10, TRANSFORMS_PER_PRIMITIVE},
        &physics[1],
        36,
        0,
        shader,
        vert_arr,
        GL_TRIANGLES,
        my_gl::Material::OBSIDIAN,
        my_gl::CollisionType::STATIC,
        nullptr,
    });

    // light
    // my_gl::GeometryObjectPrimitive{
    //     std::span<my_gl::TransformData>{transforms.data() + 12, 3},
    //     nullptr,
    //     36,
    //     0,
    //     light_shader,
    //     vertex_arr_light,
    //     GL_TRIANGLES,
    //     my_gl::Material::NO_MATERIAL,
    //     my_gl::CollisionType::GHOST,
    //     nullptr
    // }
}

void populate_tile_primitives(
    std::vector<my_gl::GeometryObjectPrimitive>& out_primitives,
    std::vector<my_gl::TransformData>& transforms,
    std::span<my_gl::Physics<float>> physics,
    const my_gl::Program& shader,
    const my_gl::VertexArray& vert_arr
)
{
    assert((out_primitives.capacity() - out_primitives.size() >= TILE_COUNT) && "Bad reserve count");

    for (u_int16_t i{0}; i < TILE_COUNT; ++i) {
        out_primitives.push_back(my_gl::GeometryObjectPrimitive{
            std::span<my_gl::TransformData>{transforms.data() + PRIMITIVE_TRANSFORM_COUNT + i * TRANSFORMS_PER_TILE, TRANSFORMS_PER_TILE},
            &physics[1],
            36,
            0,
            shader,
            vert_arr,
            GL_TRIANGLES,
            my_gl::Material::GOLD,
            my_gl::CollisionType::STATIC,
            nullptr,
        });
    }

}
