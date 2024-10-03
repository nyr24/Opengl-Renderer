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



int main() {
    std::unique_ptr<my_gl::Window> window_ptr{ my_gl::init() };
    glfwSetKeyCallback(window_ptr->ptr_raw(), key_callback);

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
    GLint localMatrixUniformLoc{ glGetUniformLocation(program, "u_local_mat" )};
    GLint windowHeightUniformLoc{ glGetUniformLocation(program, "u_window_height") };

    // constants
    constexpr float loopDuration = 5.0f;
    constexpr float objectHeight{ 0.65f };

    // static uniforms
    glUseProgram(program);
    glUniform1f(windowHeightUniformLoc, window_ptr->height());
    glUseProgram(0);

    glViewport(0, 0, window_ptr->width(), window_ptr->height());
    glfwSwapInterval(1);

    // animations
    my_gl::Translation translation_1{
        my_gl_math::Vec3<float>{ -0.4f, -0.5f, 0.0f },
        my_gl_math::Vec3<float>{ 0.4f, 0.5f, 0.0f },
        my_gl_math::Vec3<float>{ 0.01f, 0.01f, 0.0f },
        true
    };

    while (!glfwWindowShouldClose(window_ptr->ptr_raw()))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

    // scale
        //my_gl_math::Vec3<float> scaleVec{ 2.5f, 2.5f, 2.5f, 2.5f };
        //my_gl_math::Matrix44<float> scalingMatrix{ my_gl_math::Matrix44<float>::scaling(scaleVec) };

        //float currTime{ static_cast<float>(glfwGetTime()) };
        //float timeFrom0toLoopDur{ fmodf(currTime, loopDuration) };
        //float timeFrom0to1{ timeFrom0toLoopDur / loopDuration };
        //float timeFrom_1to1{ timeFrom0to1 * 2 - 1 };

    // rotation
        my_gl_math::Matrix44<float> rotationMatrix{ my_gl_math::Matrix44<float>::rotation(
            90.0f, 
            my_gl_math::Global::z
        )}; 

    // translation
        //my_gl_math::Vec3<float> translateVec{ 0.0f, 0.0f, 0.0f };
        //my_gl_math::Matrix44<float> translateMatrix{ my_gl_math::Matrix44<float>::translation(translateVec) };

        const my_gl_math::Matrix44<float>* const localMatrix{ translation_1.update() };

        if (localMatrix) {
            glUniformMatrix4fv(localMatrixUniformLoc, 1, GL_TRUE, localMatrix->data());
        }

        glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);
        glEnableVertexAttribArray(positionAttribLocation);
        glVertexAttribPointer(positionAttribLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        //rotationData.angle += rotationData.delta;
        //if (rotationData.angle > 360.0f || rotationData.angle < 0.0f) {
        //    rotationData.delta *= -1.0f;
        //}

        //translationData.val += translationData.delta;
        //if ((translationData.val > (1.0f - 0.4f)) || (translationData.val < (-1.0f + 0.25f))) {
        //    translationData.delta *= -1;
        //}

        //printf("translation: %f\n", translationData.val);
        //printf("rotation: %f\n", rotationData.angle);

        glfwSwapBuffers(window_ptr->ptr_raw());
        glfwPollEvents();
    }

    return 0;
}