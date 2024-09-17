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

    constexpr std::size_t bufferSize{ 24 };
    constexpr float vertexPositions[bufferSize] = {
        0.25f, 0.25f, 0.0f, 1.0f,
        0.25f, -0.25f, 0.0f, 1.0f,
        -0.25f, -0.25f, 0.0f, 1.0f,
        // colors
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
    };

    GLuint positionBufferId;
    my_gl::initVertexBuffer(&positionBufferId, vertexPositions, bufferSize, GL_STATIC_DRAW);

    GLuint program{ my_gl::createProgram("../shaders/vertShader.vert", "../shaders/fragShaderColorVarying.frag") };
    glUseProgram(program);

    GLint positionAttribLocation{ glGetAttribLocation(program, "position") };
    GLint colorAttribLocaiton{ glGetAttribLocation(program, "color") };

    GLint offsetUniformLocation{ glGetUniformLocation(program, "u_offset") };
    float offsetUniformValue[]{ 0.0f, 0.0f };
    float offsetVelocity[]{ 0.01f, 0.025f };

    //GLint colorOneUniformLocation{ glGetUniformLocation(program, "u_colorOne") };
    //const float colorOneUniformValue[4]{ 0.0f, 1.0f, 0.0f, 1.0f };
    //GLint colorTwoUniformLocation{ glGetUniformLocation(program, "u_colorTwo") };
    //const float colorTwoUniformValue[4]{ 0.0f, 0.0f, 1.0f, 1.0f };
    //GLint windowHeightUniformLocation{ glGetUniformLocation(program, "u_windowHeight") };

    glViewport(0, 0, window_ptr.get()->width(), window_ptr.get()->height());
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window_ptr.get()->ptr_raw()))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        //glUniform4fv(colorOneUniformLocation, 1, colorOneUniformValue);
        //glUniform4fv(colorTwoUniformLocation, 1, colorTwoUniformValue);
        //glUniform1f(windowHeightUniformLocation, window_ptr.get()->height());
        glUniform2fv(offsetUniformLocation, 1, offsetUniformValue);

        glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);
        glEnableVertexAttribArray(positionAttribLocation);
        glEnableVertexAttribArray(colorAttribLocaiton);
        glVertexAttribPointer(positionAttribLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribPointer(colorAttribLocaiton, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(48));

        glDrawArrays(GL_TRIANGLES, 0, 3);

        offsetUniformValue[0] += offsetVelocity[0];
        offsetUniformValue[1] += offsetVelocity[1];

        if ((vertexPositions[0] + offsetUniformValue[0] >= 2.0f)
            || vertexPositions[8] + offsetUniformValue[0] <= -2.0f)
            offsetVelocity[0] *= -1;
        if ((vertexPositions[1] + offsetUniformValue[1] >= 2.0f)
            || vertexPositions[9] + offsetUniformValue[1] <= -2.0f)
            offsetVelocity[1] *= -1;

        glfwSwapBuffers(window_ptr.get()->ptr_raw());
        glfwPollEvents();
    }

    return 0;
}