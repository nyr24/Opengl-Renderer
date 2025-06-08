#pragma once
#include "geometryObject.hpp"
#include "physics.hpp"
#include "renderer.hpp"
#include <vector>
#include <cstdint>
#include <span>

struct GameState {
    std::span<my_gl::TransformData>             transforms;
    std::span<my_gl::Physics<float>>            physics;
    std::span<my_gl::GeometryObjectPrimitive>   primitives;
};

constexpr float HOR_WALL_OFFSET = 4.5f;
constexpr float VERT_WALL_OFFSET = 6.0f;
constexpr float HOR_WALL_HEIGHT = 1.5f;
constexpr float VERT_WALL_WIDTH = 1.5f;
constexpr float HOR_WALL_WIDTH = VERT_WALL_OFFSET * 2.0f - VERT_WALL_WIDTH;
constexpr float VERT_WALL_HEIGHT = HOR_WALL_OFFSET * 2.0f + HOR_WALL_HEIGHT;
constexpr float PADDLE_MOVE_STEP_X = 0.9f;
constexpr float PADDLE_MOVE_STEP_Y = 0.6f;
constexpr float BALL_INIT_OFFSET_Y = -1.5f;

constexpr uint8_t TILE_ROWS = 4;
constexpr uint8_t TILE_COLS = 4;
constexpr uint8_t TILE_COUNT = TILE_ROWS * TILE_COLS;
constexpr uint8_t TRANSFORMS_PER_TILE = 2;
constexpr uint8_t TILE_TRANSFORM_COUNT = TILE_COUNT * TRANSFORMS_PER_TILE;
constexpr float TILE_WIDTH = 1.5f;
constexpr float TILE_HEIGHT = 0.6f;
constexpr float TILE_OFFSET_X = TILE_WIDTH + 0.4f;
constexpr float TILE_OFFSET_Y = TILE_HEIGHT + 0.2f;
constexpr float TILE_ROW_WIDTH = TILE_COLS * TILE_WIDTH + TILE_COLS * (TILE_OFFSET_X - TILE_WIDTH);
constexpr float TILES_INIT_LEFT_OFFSET = -TILE_ROW_WIDTH / 2.0f + 1.0f;
constexpr float TILES_INIT_TOP_OFFSET = HOR_WALL_OFFSET - HOR_WALL_HEIGHT;

enum PrimitiveIndex : uint16_t {
    BALL,
    PADDLE,
    LEFT_WALL,
    RIGHT_WALL,
    TOP_WALL,
    BOTTOM_WALL,
    // LIGHT,
    COUNT,
};

constexpr uint8_t TRANSFORMS_PER_PRIMITIVE = 2;
constexpr uint8_t PRIMITIVE_TRANSFORM_COUNT = PrimitiveIndex::COUNT * TRANSFORMS_PER_PRIMITIVE;

void process_keyboard_input(GameState* game_state, int32_t key_pressed);
void populate_primitive_transforms(std::vector<my_gl::TransformData>& out_transforms);
void populate_tile_transforms(std::vector<my_gl::TransformData>& out_transforms);
void populate_primitives(
    std::vector<my_gl::GeometryObjectPrimitive>& out_primitives,
    std::vector<my_gl::TransformData>& transforms,
    std::span<my_gl::Physics<float>> physics,
    const my_gl::Program& shader,
    const my_gl::VertexArray& vert_arr
);
void populate_tile_primitives(
    std::vector<my_gl::GeometryObjectPrimitive>& out_primitives,
    std::vector<my_gl::TransformData>& transforms,
    std::span<my_gl::Physics<float>> physics,
    const my_gl::Program& shader,
  const my_gl::VertexArray& vert_arr
);
