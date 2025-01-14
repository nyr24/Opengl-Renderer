#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include "animation.hpp"
#include "matrix.hpp"
#include "vec.hpp"
#include "utils.hpp"
#include "window.hpp"
#include "renderer.hpp"
#include "geometryObject.hpp"
#include "texture.hpp"


int main() {
    my_gl::Window window{ my_gl::init_window() };

    constexpr uint16_t texture_offset{ sizeof(float) * 3 * 4 * 6 };
    constexpr uint16_t color_offset{ texture_offset + sizeof(float) * 2 * 4 * 6 };

    my_gl::Program program1{
        "shaders/vertShader.glsl",
        "shaders/fragShader.glsl",
        {
            { .name = "a_position", .gl_type = GL_FLOAT, .count = 3, .byte_stride = 0, .byte_offset = 0 },
            { .name = "a_color", .gl_type = GL_FLOAT, .count = 4, .byte_stride = 0, .byte_offset = color_offset },
        },
        {
            { .name = "u_mvp_mat" },
        }
    };

    my_gl::Program program2{
        "shaders/vertShaderTexture.glsl",
        "shaders/fragShaderTexture.glsl",
        {
            { .name = "a_position", .gl_type = GL_FLOAT, .count = 3, .byte_stride = 0, .byte_offset = 0 },
            { .name = "a_tex_coord", .gl_type = GL_FLOAT, .count = 2, .byte_stride = 0, .byte_offset = texture_offset }
        },
        {
            { .name = "u_mvp_mat" },
            { .name = "u_tex_data1" },
            { .name = "u_tex_data2" },
            { .name = "u_lerp" }
        }
    };

// move this to object to dynamically assign uniform value,
//this would be overwritten if specified more textures than uniforms
    std::vector<my_gl::Texture> textures = {
        { "res/mine_red.jpg", program2, program2.get_uniform("u_tex_data1"), 0, GL_TEXTURE0 },
        { "res/mine_green.jpg", program2, program2.get_uniform("u_tex_data2"), 1, GL_TEXTURE1 },
    };

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

    std::vector<float> vertices = {
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

    std::vector<uint16_t> indices = {
        0, 1, 2,        0, 2, 3,
        4, 6, 5,        4, 7, 6,
        8, 9, 10,       9, 11, 10,
        12, 14, 13,     13, 14, 15,
        16, 17, 18,     17, 19, 18,
        20, 22, 21,     21, 22, 23
    };

    std::vector<const my_gl::Program*> programs{ &program1, &program2 };

    my_gl::VertexArray vertex_arr{
        std::move(vertices),
        std::move(indices),
        programs
    };

    std::vector<my_gl::Animation<float>> cube1_anims = {
        {
            my_gl::TRANSLATE,
            5.0f,
            { -1.0f, 1.0f, 0.0f },
            { -1.0f, -1.0f, 0.0f },
            my_gl::Bezier_curve_type::EASE_IN,
            my_gl::Loop_type::INVERT
        },
        {
            0.0f,
            360.0f,
            5.0f,
            my_gl_math::Global::AXIS::Y,
            my_gl::Bezier_curve_type::EASE_IN_OUT,
            my_gl::Loop_type::INVERT
        },
    };

    std::vector<my_gl_math::Matrix44<float>> cube2_transforms = {
        my_gl_math::Matrix44<float>::translation({ 0.4f, 1.0f, 0.0f })
    };

    std::vector<my_gl::Animation<float>> cube2_anims = {
        {
            0.0f,
            360.0f,
            5.0f,
            my_gl_math::Global::AXIS::Y,
            my_gl::Bezier_curve_type::EASE_IN_OUT,
            my_gl::Loop_type::INVERT
        },
        {
            my_gl::TRANSLATE,
            5.0f,
            { 1.0f, 1.0f, 0.0f },
            { 1.0f, -1.0f, 0.0f },
            my_gl::Bezier_curve_type::EASE_IN,
            my_gl::Loop_type::INVERT
        },
    };


    my_gl::GeometryObject cube1{ std::move(cube1_anims), 36, 0, program1, vertex_arr, GL_TRIANGLES, {} };
    my_gl::GeometryObject cube2{ std::move(cube2_anims), 36, 0, program2, vertex_arr, GL_TRIANGLES, { &textures[0], &textures[1] } };

    std::vector<my_gl::GeometryObject> objects{ std::move(cube1), std::move(cube2) };
    my_gl::ObjectCache object_cache{};

    auto view_mat{ my_gl_math::Matrix44<float>::translation(
        my_gl_math::Vec3<float>{ 0.0f, 0.0f, -6.0f }
    )};

    auto projection_mat{ my_gl_math::Matrix44<float>::perspective_fov(
        45.0f, static_cast<float>(window.width()) / window.height(), 0.1f, 50.0f
    )};

    auto projection_view_mat{ projection_mat * view_mat };

    my_gl::Renderer renderer{
        std::move(objects),
        std::move(projection_view_mat)
    };

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window.ptr_raw())) {
        auto start_frame{ std::chrono::steady_clock::now() };

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.render(object_cache);

        glfwSwapBuffers(window.ptr_raw());
        glfwPollEvents();

        my_gl::Duration_sec frame_duration{ std::chrono::steady_clock::now() - start_frame };

        renderer.update_time(frame_duration);
    }

    return 0;
}
