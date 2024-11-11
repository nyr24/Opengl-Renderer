#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <string_view>
#include "window.hpp"
#include <memory>

namespace my_gl {
    my_gl::Window init_window();
    void          init_GLFW();
    void          init_GLEW();
    GLuint        create_shader(GLenum shaderType, const char* filePath);
    GLuint        create_program(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);    
    void          callback_debug_message(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data);
    void          callback_framebuffer_size(GLFWwindow* window, int width, int height);
    void          callback_keyboard(GLFWwindow* window, int key, int scancode, int action, int mode);
}