#pragma once
#include "geometryObject.hpp"
#include "physics.hpp"
#include <span>

struct GameState {
    std::span<my_gl::TransformData>     transforms;
    std::span<my_gl::Physics<float>>    physics;
};

void process_keyboard_input(GameState* game_state, int32_t key_pressed);

constexpr float WALL_WIDTH = 7.0f;
constexpr float WALL_HEIGHT = 1.0f;
constexpr float WALL_Y_OFFSET = 3.0f;
constexpr float WALL_X_OFFSET = 3.0f;
constexpr float PADDLE_MOVE_STEP = 0.5f;
