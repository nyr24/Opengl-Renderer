#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <variant>
#include "window.hpp"

struct GameState;

namespace my_gl {
    my_gl::Window   init_window();
    void            init_user_state(GLFWwindow* window, GameState *game_state);
    void            init_GLFW();
    void            init_GLEW();
    GLuint          create_shader(GLenum shaderType, const char* filePath);
    GLuint          create_program(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);
    void            callback_debug_message(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data);
    void            callback_framebuffer_size(GLFWwindow* window, int width, int height);
    void            callback_keyboard(GLFWwindow* window, int key, int scancode, int action, int mode);
    void            callback_mouse_move(GLFWwindow* window, double xpos, double ypos);
    void            callback_scroll(GLFWwindow* window, double xoffset, double yoffset);
    void            print_max_vert_attrs_supported();
}

enum NoneT {
    None
};

template<typename T>
struct Option {
    using InnerType = T;

    bool is_none() const {
        return this->get_if() == nullptr;
    }

    bool is_some() const {
        return this->get_if() != nullptr;
    }

    T* get_if() {
        return std::get_if<T>(&tagged_union);
    }

    const T* get_if() const {
        return std::get_if<T>(&tagged_union);
    }

    operator bool() const { return this->is_some(); }

private:
    std::variant<NoneT, T> tagged_union;
};
