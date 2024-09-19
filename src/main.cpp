#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include <memory>
#include "utils.h"
#include "window.h"
#include "programState.h"


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

    constexpr std::size_t bufferSize{ 12 };
    constexpr float vertexPositions[bufferSize] = {
        0.25f, 0.25f, 0.0f, 1.0f,
        0.25f, -0.25f, 0.0f, 1.0f,
        -0.25f, -0.25f, 0.0f, 1.0f,
        // colors
        //1.0f, 0.0f, 0.0f, 1.0f,
        //0.0f, 1.0f, 0.0f, 1.0f,
        //0.0f, 0.0f, 1.0f, 1.0f,
    };

    GLuint positionBufferId;
    my_gl::initVertexBuffer(&positionBufferId, vertexPositions, bufferSize, GL_STATIC_DRAW);

    my_gl::ProgramState program{ "../shaders/vertShader.vert", "../shaders/fragShaderLerp.frag" };
    program.use();

    const std::array<float, 1> windowHeightUniformValue{ static_cast<float>(window_ptr.get()->height()) };
    constexpr std::array<float, 4> colorOneUniformValue{ 0.0f, 1.0f, 0.0f, 1.0f };
    constexpr std::array<float, 4> colorTwoUniformValue{ 0.0f, 0.0f, 1.0f, 1.0f };
    std::array<float, 2> offsetUniformValue{ 0.0f, 0.0f };
    std::array<float, 2> offsetVelocity{ 0.01f, 0.025f };
    
    program.setAttrib("position");
    program.setUniform<4>("u_colorOne", colorOneUniformValue);
    program.setUniform<4>("u_colorTwo", colorTwoUniformValue);
    program.setUniform<1>("u_windowHeight", windowHeightUniformValue);
    program.setUniform<2>("u_offset", offsetUniformValue);

    const auto* const colorOneUniform{ program.getUniform<4>("u_colorOne") };
    const auto* const colorTwoUniform{ program.getUniform<4>("u_colorTwo") };
    const auto* const windowHeightUniform{ program.getUniform<1>("u_windowHeight") };
    const auto* const offsetUniform{ program.getUniform<2>("u_offset") };

    const auto* const positionAttrib{ program.getAttrib("position") };

    glViewport(0, 0, window_ptr.get()->width(), window_ptr.get()->height());
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window_ptr.get()->ptr_raw()))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        program.use();

        program.useUniform<4>(colorOneUniform->name);
        program.useUniform<4>(colorTwoUniform->name);
        program.useUniform<1>(windowHeightUniform->name);
        program.useUniform<2>(offsetUniform->name);

        glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);
        glEnableVertexAttribArray(positionAttrib->location);
        glVertexAttribPointer(positionAttrib->location, 4, GL_FLOAT, GL_FALSE, 0, 0);

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