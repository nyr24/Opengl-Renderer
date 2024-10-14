#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include <memory>
#include "utils.hpp"
#include "window.hpp"
#include "mat.hpp"
#include "vec.hpp"
#include "animation.hpp"
#include "program.hpp"

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

    my_gl::Program program{ my_gl::Program("../shaders/vertShader.vert", "../shaders/fragShader.frag") };

    program.set_attrib("position");
    program.set_uniform("u_local_mat");
    program.set_uniform("u_proj_mat");
    program.set_uniform("u_elapsed_time");
    program.set_uniform("u_loop_duration");

    glfwSetKeyCallback(window.ptr_raw(), key_callback);

    GLuint vaoId;
    my_gl::initVertexArrayObject(&vaoId);

// vertices
    #define LEFT_BOTTOM_NEAR     -1.0f, -1.0f, 1.0f, 1.0f 
    #define LEFT_BOTTOM_FAR      -1.0f, -1.0f, -1.0f, 1.0f 
    #define LEFT_TOP_NEAR        -1.0f, 1.0f, 1.0f, 1.0f 
    #define LEFT_TOP_FAR         -1.0f, 1.0f, -1.0f, 1.0f 
    #define RIGHT_BOTTOM_NEAR    1.0f, -1.0f, 1.0f, 1.0f 
    #define RIGHT_BOTTOM_FAR     1.0f, -1.0f, -1.0f, 1.0f 
    #define RIGHT_TOP_NEAR       1.0f, 1.0f, 1.0f, 1.0f
    #define RIGHT_TOP_FAR        1.0f, 1.0f, -1.0f, 1.0f 

// colors
    #define COLOR_RED            0.9f, 0.15f, 0.0f, 1.0f 
    #define COLOR_GREEN          0.0f, 0.9f, 0.05f, 1.0f 
    #define COLOR_BLUE           0.0f, 0.05f, 0.9f, 1.0f 
    #define COLOR_YELLOW         0.55f, 0.45f, 0.0f, 1.0f 
    #define COLOR_ORANGE         0.8f, 0.35f, 0.0f, 1.0f 
    #define COLOR_PURPLE         0.25f, 0.0f, 0.85f, 1.0f 
    
    constexpr std::size_t bufferSize{ sizeof(float) * 4 * 3 * 12 };

    constexpr std::array<float, bufferSize> vertex_positions = {
        // vertices
        LEFT_BOTTOM_NEAR,
        LEFT_TOP_NEAR,
        LEFT_BOTTOM_FAR,
        LEFT_TOP_FAR,
        RIGHT_BOTTOM_NEAR,
        RIGHT_TOP_NEAR,
        RIGHT_BOTTOM_FAR,
        RIGHT_TOP_FAR,

        // colors
        COLOR_RED,
        COLOR_GREEN,
        COLOR_BLUE,
        COLOR_YELLOW,
        COLOR_ORANGE,
        COLOR_PURPLE
    };

    constexpr std::array<unsigned int, 3 * 12> indeces = {
        
    };

    GLuint positionBufferId;
    my_gl::initVertexBuffer(&positionBufferId, vertex_positions.data(), vertex_positions.size(), GL_STATIC_DRAW);

    // constants
    constexpr float loop_dur = 3.0f;

    // static uniforms
    program.use();
    glUniform1f(program.get_uniform_loc("u_loop_duration"), loop_dur);
    program.un_use();

    glViewport(0, 0, window.width(), window.height());
    glfwSwapInterval(1);

    auto rotation_mat{ my_gl_math::Matrix44<float>::rotation3d(
        my_gl_math::Vec3<float>{ 11.0f, 11.0f, 0.0f }
    ) };
    auto scale_mat{ my_gl_math::Matrix44<float>::scaling(
        my_gl_math::Vec3<float>{ 0.6f, 0.6f, 1.0f }
    ) };

    auto local_mat{ scale_mat * rotation_mat };

    while (!glfwWindowShouldClose(window.ptr_raw()))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.use();

        const auto elapsed_time{ static_cast<float>(glfwGetTime()) };
        glUniform1f(program.get_uniform_loc("u_elapsed_time"), elapsed_time);

        const float aspect = window.width() / window.height();

        auto proj_mat{ 
            my_gl_math::Matrix44<float>::perspective_fov(45.0f, aspect, 0.1f, 5.0f) 
        };

        glUniformMatrix4fv(program.get_uniform_loc("u_local_mat"), 1, true, local_mat.data());
        glUniformMatrix4fv(program.get_uniform_loc("u_proj_mat"), 1, true, proj_mat.data());

        glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);
        glEnableVertexAttribArray(program.get_attrib_loc("position"));
        glVertexAttribPointer(program.get_attrib_loc("position"), 4, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

        glfwSwapBuffers(window.ptr_raw());
        glfwPollEvents();
    }

    return 0;
}