#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include "utils.hpp"
#include "window.hpp"
#include "mat.hpp"
#include "vec.hpp"
#include "animation.hpp"
#include "renderer.hpp"
#include "geometryObject.hpp"

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        //we close GLFW by setting its WindowShouldClose 
        //property to true.
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}


int main() {
    my_gl::Window window{ my_gl::init() };

    std::vector<my_gl::Attribute> attributes = {
        my_gl::Attribute{ .name = "position", .gl_type = GL_FLOAT, .count = 3, .byte_stride = 0, .byte_offset = 0 },
        my_gl::Attribute{ .name = "color", .gl_type = GL_FLOAT, .count = 4, .byte_stride = 0, .byte_offset = sizeof(float) * 3 * 24 }
    };

    std::vector<my_gl::Uniform> uniforms = {
        { .name = "u_mvp_mat" },
    };

    my_gl::Program program{ my_gl::Program(
        "../shaders/vertShader.vert", 
        "../shaders/fragShader.frag",
        std::move(attributes),
        std::move(uniforms)
    )};

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

    std::vector<float> vertices = {
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

    my_gl::VertexArray vertex_arr{ my_gl::VertexArray(
        std::move(vertices),
        std::move(indices),
        program
    )};

    glViewport(0, 0, window.width(), window.height());
    glfwSwapInterval(1);

    // geometry objects
    std::vector<my_gl::Animation<float>> cube1_anims = {
        {
            my_gl::ROTATE3d,
            { 0.0f, 0.0f, 0.0f },
            { 180.0f, 180.0f, 0.0f },
            { 0.01f, 0.01f, 0.0f },
            true
        },
        {
            my_gl::TRANSLATE,
            { 1.0f, 1.0f, -2.0f },
            { -1.0f, -1.0f, 2.0f },
            { -0.01f, -0.01f, 0.01f },
            true
        },
    };

    std::vector<my_gl::Animation<float>> cube2_anims = { 
        {
            my_gl::ROTATE3d,
            { 0.0f, 0.0f, 0.0f },
            { 180.0f, 180.0f, 0.0f },
            { 0.01f, 0.01f, 0.0f },
            true
        },
        {
            my_gl::TRANSLATE,
            { -1.0f, -1.0f, 2.0f },
            { 1.0f, 1.0f, -2.0f },
            { 0.01f, 0.01f, -0.01f },
            true
        },
    };

    my_gl::Cube cube1{ std::move(cube1_anims), 36, 0 };
    my_gl::Cube cube2{ std::move(cube2_anims), 36, 0 };

    std::vector<my_gl::IGeometry_object*> objects{ &cube1, &cube2 };

    auto view_mat{ my_gl_math::Matrix44<float>::translation(
        my_gl_math::Vec3<float>{ 0.0f, 0.0f, -4.0f }
    )};

    auto projection_mat{ my_gl_math::Matrix44<float>::perspective_fov(
        45.0f, window.width() / window.height(), 0.1f, 50.0f
    )};

    // renderer
    my_gl::Renderer renderer{ 
        std::move(objects),
        std::move(projection_mat),
        std::move(view_mat),
        program,
        vertex_arr
    };

    while (!glfwWindowShouldClose(window.ptr_raw()))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.use();
        vertex_arr.bind();
        renderer.render();
    
        glfwSwapBuffers(window.ptr_raw());
        glfwPollEvents(); 
    }

    program.un_use();

    return 0;
}