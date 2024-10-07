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

void get_translation(const my_gl_math::Vec3<float>& vec) {
    my_gl_math::Matrix44<float> tr_mat{ my_gl_math::Matrix44<float>::translation(vec) };
}

int main() {
    my_gl::Window window{ my_gl::init() };

    glfwSetKeyCallback(window.ptr_raw(), key_callback);

    GLuint vaoId;
    my_gl::initVertexArrayObject(&vaoId);

    constexpr std::size_t bufferSize{ sizeof(float) * 4 * 3 };
    constexpr float vertexPositions[bufferSize] = {
        -0.25f, -0.25f, 0.0f, 1.0f,
        0.0f, +0.4f, 0.0f, 1.0f,
        +0.25f, -0.25f, 0.0f, 1.0f,
    };

    GLuint positionBufferId;
    my_gl::initVertexBuffer(&positionBufferId, vertexPositions, bufferSize, GL_STATIC_DRAW);

    GLuint program{ my_gl::createProgram("../shaders/vertShader.vert", "../shaders/fragShader.frag") };

    GLint positionAttribLocation{ glGetAttribLocation(program, "position") };
    GLint unif_trans_mat_loc{ glGetUniformLocation(program, "u_translation_mat" )};
    GLint unif_rot_mat_loc{ glGetUniformLocation(program, "u_rotation_mat" )};
    GLint unif_scale_mat_loc{ glGetUniformLocation(program, "u_scaling_mat" )};
    GLint unif_elapsed_time_loc{ glGetUniformLocation(program, "u_elapsed_time") };
    GLint unif_loop_dur_loc{ glGetUniformLocation(program, "u_loop_duration") };

    // constants
    constexpr float loop_dur = 3.0f;

    // static uniforms
    glUseProgram(program);
    glUniform1f(unif_loop_dur_loc, loop_dur);
    glUseProgram(0);

    glViewport(0, 0, window.width(), window.height());
    glfwSwapInterval(1);

    // animations
    my_gl::Translation translation_anim{
        my_gl_math::Vec3<float>{ -0.5f, -0.5f, 0.0f },
        my_gl_math::Vec3<float>{ 0.5f, 0.5f, 0.0f },
        my_gl_math::Vec3<float>{ 0.01f, 0.01f, 0.0f },
        true
    };

    my_gl::Rotation3d rotation3d_anim{
        my_gl_math::Vec3<float>{ 0.0f, 0.0f, 0.0f },
        my_gl_math::Vec3<float>{ 0.0f, 0.0f, -5.0f },
        my_gl_math::Vec3<float>{ 0.0f, 0.0f, -0.01f },
        true
    };


    my_gl::Scaling scaling_anim {
        my_gl_math::Vec3<float>{ 1.0f, 1.0f, 1.0f },
        my_gl_math::Vec3<float>{ 1.5f, 1.5f, 1.0f },
        my_gl_math::Vec3<float>{ 0.01f, 0.01f, 0.0f },
        true
    };

    my_gl::Rotation rotation_anim{
        30.0f,
        90.0f,
        0.01f,
        my_gl_math::Global::X,
        true
    };

    while (!glfwWindowShouldClose(window.ptr_raw()))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        const auto elapsed_time{ static_cast<float>(glfwGetTime()) };
        glUniform1f(unif_elapsed_time_loc, elapsed_time);

        //glUniformMatrix4fv(unif_trans_mat_loc, 1, GL_TRUE, translation_anim.update().data());
        glUniformMatrix4fv(unif_rot_mat_loc, 1, GL_TRUE, rotation_anim.update().data());
        //glUniformMatrix4fv(unif_scale_mat_loc, 1, GL_TRUE, scaling_anim.update().data());

        glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);
        glEnableVertexAttribArray(positionAttribLocation);
        glVertexAttribPointer(positionAttribLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window.ptr_raw());
        glfwPollEvents();
    }

    return 0;
}