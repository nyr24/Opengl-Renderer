#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include "utils.hpp"
#include "window.hpp"
#include "renderer.hpp"
#include "geometryObject.hpp"
#include "texture.hpp"

int main() {
    my_gl::Window window{ my_gl::init_window() };

    std::vector<my_gl::Attribute> attributes = {
        { .name = "a_position", .gl_type = GL_FLOAT, .count = 3, .byte_stride = 0, .byte_offset = 0 },
        /* { .name = "a_color", .gl_type = GL_FLOAT, .count = 4, .byte_stride = 0, .byte_offset = sizeof(float) * 3 * 24 }, */
        { .name = "a_tex_coord", .gl_type = GL_FLOAT, .count = 2, .byte_stride = 0, .byte_offset = (sizeof(float) * 3 * 4 * 6) }
    };

    std::vector<my_gl::Uniform> uniforms = {
        { .name = "u_mvp_mat" },
        { .name = "u_tex_data1" },
        { .name = "u_tex_data2" }
    };

    my_gl::Program program{ my_gl::Program(
        "../../shaders/vertShader.vert", 
        "../../shaders/fragShader.frag",
        std::move(attributes),
        std::move(uniforms)
    )};

// move this to object to dynamically assign uniform value,
//this would be overwritten if specified more textures than uniforms
    std::vector<my_gl::Texture> textures = {
        { "../../res/mine_red.jpg", program, program.get_uniform("u_tex_data1"), 0, GL_TEXTURE0 },
        { "../../res/mine_green.jpg", program, program.get_uniform("u_tex_data2"), 1, GL_TEXTURE1 },
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
        /* GREEN, GREEN, GREEN, GREEN,
        // back
        YELLOW, YELLOW, YELLOW, YELLOW,
        // right
        BLUE, BLUE, BLUE, BLUE,
        // left
        RED, RED, RED, RED,
        // top
        ORANGE, ORANGE, ORANGE, ORANGE,
        // bottom
        PURPLE, PURPLE, PURPLE, PURPLE, */
    };

    std::vector<uint16_t> indices = {
        0, 1, 2,        0, 2, 3,
        4, 6, 5,        4, 7, 6,
        8, 9, 10,       9, 11, 10,
        12, 14, 13,     13, 14, 15,
        16, 17, 18,     17, 19, 18,
        20, 22, 21,     21, 22, 23
    };

    my_gl::VertexArray vertex_arr{ my_gl::VertexArray(
        std::move(vertices),
        std::move(indices),
        program
    )};

    std::vector<my_gl::Animation<float>> cube1_anims = {
        {
            my_gl::TRANSLATE,
            5.0f,
            { 1.0f, 1.0f, -2.0f },
            { -1.0f, -1.0f, 1.0f },
            my_gl::Bezier_curve_type::EASE_IN,
            my_gl::Loop_type::INVERT
        },
        {
            my_gl::ROTATE3d,
            5.0f,
            { 0.0f, 0.0f, 0.0f },
            { 0.0f, 10.0f, 0.0f },
            my_gl::Bezier_curve_type::EASE_IN,
            my_gl::Loop_type::INVERT
        },
        {
            my_gl::SCALE,
            5.0f,
            { 1.0f, 1.0f, 1.0f },
            { 2.0f, 2.0f, 2.0f },
            my_gl::Bezier_curve_type::EASE_IN,
            my_gl::Loop_type::INVERT
        }
    };

    std::vector<my_gl::Animation<float>> cube2_anims = { 
        {
            my_gl::TRANSLATE,
            5.0f,
            { 1.0f, -1.0f, 1.0f },
            { -1.0f, 1.0f, -2.0f },
            my_gl::Bezier_curve_type::EASE_OUT,
            my_gl::Loop_type::INVERT
        },
        {
            my_gl::ROTATE3d,
            5.0f,
            { 10.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f },
            my_gl::Bezier_curve_type::EASE_OUT,
            my_gl::Loop_type::INVERT
        },
        {
            my_gl::SCALE,
            5.0f,
            { 2.0f, 2.0f, 2.0f },
            { 1.0f, 1.0f, 1.0f },
            my_gl::Bezier_curve_type::EASE_OUT,
            my_gl::Loop_type::INVERT
        }
    };

    std::vector<const my_gl::Texture*> cube1_textures = { &textures[0] };
    std::vector<const my_gl::Texture*> cube2_textures = { &textures[0], &textures[1] };

    my_gl::GeometryObject cube1{ std::move(cube1_anims), 36, 0, program, vertex_arr, GL_TRIANGLES, std::move(cube1_textures) };
    my_gl::GeometryObject cube2{ std::move(cube2_anims), 36, 0, program, vertex_arr, GL_TRIANGLES, std::move(cube2_textures) };

    std::vector<my_gl::GeometryObject> objects{ cube1, cube2 };

    auto view_mat{ my_gl_math::Matrix44<float>::translation(
        my_gl_math::Vec3<float>{ 0.0f, 0.0f, -4.0f }
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

        renderer.render();

        glfwSwapBuffers(window.ptr_raw());
        glfwPollEvents();

        my_gl::Duration_sec frame_duration{ std::chrono::steady_clock::now() - start_frame };

        renderer.update_time(frame_duration);
    }

    program.un_use();

    return 0;
}
