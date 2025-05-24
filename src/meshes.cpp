#include "meshes.hpp"
#include "matrix.hpp"
#include <array>
#include <cstdint>

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

        // normals
#define NORMAL_TOP          0.0f, 1.0f, 0.0f
#define NORMAL_BOTTOM       0.0f, -1.0f, 0.0f
#define NORMAL_RIGHT        1.0f, 0.0f, 0.0f
#define NORMAL_LEFT         -1.0f, 0.0f, 0.0f
#define NORMAL_FORWARD      0.0f, 0.0f, 1.0f
#define NORMAL_BACKWARD     0.0f, 0.0f, -1.0f

        // colors
#define WHITE   1.0f, 1.0f, 1.0f
#define BLACK   0.0f, 0.0f, 0.0f
#define RED     1.0f, 0.0f, 0.0f
#define GREEN   0.0f, 1.0f, 0.0f
#define BLUE    0.0f, 0.0f, 1.0f
#define ORANGE  1.0f, 0.5f, 0.1f
#define PURPLE  0.5f, 0.0f, 0.5f
#define YELLOW  1.0f, 1.0f, 0.0f

        // textures
#define TEX_LEFT_TOP       0.0f, 1.0f
#define TEX_MIDDLE_TOP     0.5f, 1.0f
#define TEX_LEFT_BOT       0.0f, 0.0f
#define TEX_RIGHT_TOP      1.0f, 1.0f
#define TEX_MIDDLE_BOT     0.5f, 0.0f
#define TEX_RIGHT_BOT      1.0f, 0.0f

        constexpr uint16_t position_count = 3 * 4 * 6;
        constexpr uint16_t texture_count = 2 * 4 * 6;
        constexpr uint16_t color_count = position_count;
        constexpr uint16_t normal_count = position_count;
        constexpr uint16_t vertice_count = position_count + texture_count + color_count + normal_count;

        std::array<float, vertice_count> cube_vertices = {
            // POSITIONS
            // front
            LEFT_TOP_NEAR,  LEFT_BOT_NEAR,  RIGHT_BOT_NEAR, RIGHT_TOP_NEAR,
            // back
            LEFT_TOP_FAR,   LEFT_BOT_FAR,   RIGHT_BOT_FAR,  RIGHT_TOP_FAR,
            // right
            RIGHT_TOP_NEAR, RIGHT_BOT_NEAR, RIGHT_TOP_FAR,  RIGHT_BOT_FAR,
            // left
            LEFT_TOP_NEAR,  LEFT_BOT_NEAR,  LEFT_TOP_FAR,   LEFT_BOT_FAR,
            // top
            LEFT_TOP_NEAR,  RIGHT_TOP_NEAR, LEFT_TOP_FAR,   RIGHT_TOP_FAR,
            // bottom
            LEFT_BOT_NEAR,  RIGHT_BOT_NEAR, LEFT_BOT_FAR,   RIGHT_BOT_FAR,

            // TEXTURES
            // front
            TEX_LEFT_TOP, TEX_LEFT_BOT, TEX_RIGHT_BOT, TEX_RIGHT_TOP,
            TEX_LEFT_TOP, TEX_LEFT_BOT, TEX_RIGHT_BOT, TEX_RIGHT_TOP,
            TEX_LEFT_TOP, TEX_LEFT_BOT, TEX_RIGHT_TOP, TEX_RIGHT_BOT,
            TEX_LEFT_TOP, TEX_LEFT_BOT, TEX_RIGHT_TOP, TEX_RIGHT_BOT,
            TEX_LEFT_TOP, TEX_LEFT_BOT, TEX_RIGHT_TOP, TEX_RIGHT_BOT,
            TEX_LEFT_TOP, TEX_LEFT_BOT, TEX_RIGHT_TOP, TEX_RIGHT_BOT,

            // WORLD COLORS
            GREEN,  GREEN,  GREEN,  GREEN,
            YELLOW, YELLOW, YELLOW, YELLOW,
            BLUE,   BLUE,   BLUE,   BLUE,
            RED,    RED,    RED,    RED,
            ORANGE, ORANGE, ORANGE, ORANGE,
            PURPLE, PURPLE, PURPLE, PURPLE,

            // NORMALS
            NORMAL_FORWARD,     NORMAL_FORWARD,     NORMAL_FORWARD,     NORMAL_FORWARD,
            NORMAL_BACKWARD,    NORMAL_BACKWARD,    NORMAL_BACKWARD,    NORMAL_BACKWARD,
            NORMAL_RIGHT,       NORMAL_RIGHT,       NORMAL_RIGHT,       NORMAL_RIGHT,
            NORMAL_LEFT,        NORMAL_LEFT,        NORMAL_LEFT,        NORMAL_LEFT,
            NORMAL_TOP,         NORMAL_TOP,         NORMAL_TOP,         NORMAL_TOP,
            NORMAL_BOTTOM,      NORMAL_BOTTOM,      NORMAL_BOTTOM,      NORMAL_BOTTOM,
        };

        std::array<uint16_t, 6 * 6> cube_indeces = {
            0, 1, 2,        0, 2, 3,
            4, 6, 5,        4, 7, 6,
            8, 9, 10,       9, 11, 10,
            12, 14, 13,     13, 14, 15,
            16, 17, 18,     17, 19, 18,
            20, 22, 21,     21, 22, 23
        };

        Boundaries cube_boundaries = {
            .ltn = { LEFT_TOP_NEAR },
            .ltf = { LEFT_TOP_FAR },
            .rtn = { RIGHT_TOP_NEAR },
            .rtf = { RIGHT_TOP_FAR },
            .lbn = { LEFT_BOT_NEAR },
            .lbf = { LEFT_BOT_FAR },
            .rbn = { RIGHT_BOT_NEAR },
            .rbf = { RIGHT_BOT_FAR },
        };

        Mesh get_cube_mesh() {
            return Mesh{
                .vertices = std::span{cube_vertices},
                .indices = std::span{cube_indeces},
                .boundaries = &cube_boundaries,
            };
        };

        Boundaries Mesh::transform_boundaries(const my_gl::math::Matrix44<float>& mat) const {
            return Boundaries{
                .ltn = mat * my_gl::math::Vec4<float>(boundaries->ltf),
                .ltf = mat * my_gl::math::Vec4<float>(boundaries->ltf),
                .rtn = mat * my_gl::math::Vec4<float>(boundaries->rtn),
                .rtf = mat * my_gl::math::Vec4<float>(boundaries->rtf),
                .lbn = mat * my_gl::math::Vec4<float>(boundaries->lbn),
                .lbf = mat * my_gl::math::Vec4<float>(boundaries->lbf),
                .rbn = mat * my_gl::math::Vec4<float>(boundaries->rbn),
                .rbf = mat * my_gl::math::Vec4<float>(boundaries->rbf),
            };
        }
    }
}
