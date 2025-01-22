#include "meshes.hpp"
#include <vector>

namespace my_gl {
    namespace meshes {
        // positions
#define LEFT_TOP_NEAR       -.5f, .5f, .5f
#define LEFT_BOT_NEAR       -.5f, -.5f, .5f
#define LEFT_TOP_FAR        -.5f, .5f, -.5f
#define LEFT_BOT_FAR        -.5f, -.5f, -.5f
#define RIGHT_TOP_NEAR      .5f, .5f, .5f
#define RIGHT_BOT_NEAR      .5f, -.5f, .5f
#define RIGHT_TOP_FAR       .5f, .5f, -.5f
#define RIGHT_BOT_FAR       .5f, -.5f, -.5f

        // colors
#define RED     1.0f, 0.0f, 0.0f, 1.0f
#define GREEN   0.0f, 1.0f, 0.0f, 1.0f
#define BLUE    0.0f, 0.0f, 1.0f, 1.0f
#define ORANGE  1.0f, 0.5f, 0.1f, 1.0f
#define PURPLE  0.5f, 0.0f, 0.5f, 1.0f
#define YELLOW  1.0f, 1.0f, 0.0f, 1.0f

        // textures
#define TEX_LEFT_TOP       0.0f, 1.0f
#define TEX_MIDDLE_TOP     0.5f, 1.0f
#define TEX_LEFT_BOT       0.0f, 0.0f
#define TEX_RIGHT_TOP      1.0f, 1.0f
#define TEX_MIDDLE_BOT     0.5f, 0.0f
#define TEX_RIGHT_BOT      1.0f, 0.0f

        std::vector<float> cube_vertices = {
            // POSITIONS
            // front
            LEFT_TOP_NEAR, LEFT_BOT_NEAR, RIGHT_BOT_NEAR, RIGHT_TOP_NEAR,
            // back
            LEFT_TOP_FAR, LEFT_BOT_FAR, RIGHT_BOT_FAR, RIGHT_TOP_FAR,
            // right
            RIGHT_TOP_NEAR, RIGHT_BOT_NEAR, RIGHT_TOP_FAR, RIGHT_BOT_FAR,
            // left
            LEFT_TOP_NEAR, LEFT_BOT_NEAR, LEFT_TOP_FAR, LEFT_BOT_FAR,
            // top
            LEFT_TOP_NEAR, RIGHT_TOP_NEAR, LEFT_TOP_FAR, RIGHT_TOP_FAR,
            // bottom
            LEFT_BOT_NEAR, RIGHT_BOT_NEAR, LEFT_BOT_FAR, RIGHT_BOT_FAR,

            // TEXTURES
            // front
            TEX_LEFT_TOP, TEX_LEFT_BOT, TEX_RIGHT_BOT, TEX_RIGHT_TOP,
            TEX_LEFT_TOP, TEX_LEFT_BOT, TEX_RIGHT_BOT, TEX_RIGHT_TOP,
            TEX_LEFT_TOP, TEX_LEFT_BOT, TEX_RIGHT_TOP, TEX_RIGHT_BOT,
            TEX_LEFT_TOP, TEX_LEFT_BOT, TEX_RIGHT_TOP, TEX_RIGHT_BOT,
            TEX_LEFT_TOP, TEX_LEFT_BOT, TEX_RIGHT_TOP, TEX_RIGHT_BOT,
            TEX_LEFT_TOP, TEX_LEFT_BOT, TEX_RIGHT_TOP, TEX_RIGHT_BOT,

            // COLORS
            // front
            GREEN, GREEN, GREEN, GREEN,
            // back
            YELLOW, YELLOW, YELLOW, YELLOW,
            // right
            BLUE, BLUE, BLUE, BLUE,
            // left
            RED, RED, RED, RED,
            // top
            ORANGE, ORANGE, ORANGE, ORANGE,
            // bottom
            PURPLE, PURPLE, PURPLE, PURPLE,
        };

        std::vector<uint16_t> cube_indeces = {
            0, 1, 2,        0, 2, 3,
            4, 6, 5,        4, 7, 6,
            8, 9, 10,       9, 11, 10,
            12, 14, 13,     13, 14, 15,
            16, 17, 18,     17, 19, 18,
            20, 22, 21,     21, 22, 23
        };

        Mesh cube_mesh{ std::move(cube_vertices), std::move(cube_indeces) };
    }
}
