#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <string_view>
#include "window.hpp"
#include <memory>

namespace my_gl {
    my_gl::Window init_window();

    void initGLFW();
    void initGLEW();

    GLuint createShader(GLenum shaderType, const char* filePath);
    GLuint createProgram(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);

    void GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data);
}