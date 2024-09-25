#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include <memory>
#include "utils.h"
#include "window.h"


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
    std::unique_ptr<my_gl::Window> window_ptr{ my_gl::init() };
    glfwSetKeyCallback(window_ptr.get()->ptr_raw(), key_callback);

    GLuint vaoId;
    my_gl::initVertexArrayObject(&vaoId);

    constexpr std::size_t bufferSize{ 4 * 6 };
    constexpr float vertexPositions[bufferSize] = {
        -0.25f, -0.25f, 0.0f, 1.0f,
        0.0f, +0.25f, 0.0f, 1.0f,
        +0.25f, -0.25f, 0.0f, 1.0f,
        
        -0.25f, -0.25f, 0.0f, 1.0f,
        0.0f, +0.25f, 0.0f, 1.0f,
        +0.25f, -0.25f, 0.0f, 1.0f,
    };

    GLuint positionBufferId;
    my_gl::initVertexBuffer(&positionBufferId, vertexPositions, bufferSize, GL_STATIC_DRAW);

    GLuint program{ my_gl::createProgram("../shaders/vertShader.vert", "../shaders/fragShader.frag") };

    GLint positionAttribLocation{ glGetAttribLocation(program, "position") };

    GLint elapsedTimeUniformLocation{ glGetUniformLocation(program, "u_elapsedTime") };
    GLint loopDurationUniformLocation{ glGetUniformLocation(program, "u_loopDuration") };

    const float loopDurationUniformValue = 5.0f;

    // static uniforms
    glUseProgram(program);
    glUniform1f(loopDurationUniformLocation, loopDurationUniformValue);

    glViewport(0, 0, window_ptr.get()->width(), window_ptr.get()->height());
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window_ptr.get()->ptr_raw()))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1f(elapsedTimeUniformLocation, static_cast<float>(glfwGetTime()));

        glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);
        glEnableVertexAttribArray(positionAttribLocation);
        glVertexAttribPointer(positionAttribLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniform1f(elapsedTimeUniformLocation, static_cast<float>(glfwGetTime()) + (loopDurationUniformValue / 2));

        glVertexAttribPointer(positionAttribLocation, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(sizeof(float) * 4 * 3));
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window_ptr.get()->ptr_raw());
        glfwPollEvents();
    }

    return 0;
}