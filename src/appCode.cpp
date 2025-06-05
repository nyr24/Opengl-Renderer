#include "appCode.hpp"
#include "geometryObject.hpp"
#include <GLFW/glfw3.h>
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
