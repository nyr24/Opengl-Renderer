#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <concepts>
#include "vec.hpp"
#include "window.hpp"

namespace my_gl {
    my_gl::Window init_window();
    void          init_GLFW();
    void          init_GLEW();
    GLuint        create_shader(GLenum shaderType, const char* filePath);
    GLuint        create_program(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);    
    void          callback_debug_message(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data);
    void          callback_framebuffer_size(GLFWwindow* window, int width, int height);
    void          callback_keyboard(GLFWwindow* window, int key, int scancode, int action, int mode);
    void          callback_mouse_move(GLFWwindow* window, double xpos, double ypos);
    void          callback_scroll(GLFWwindow* window, double xoffset, double yoffset);
    void          print_max_vert_attrs_supported();

    template<typename Tag, typename Value>
    struct TaggedUnion {
        const TaggedUnion<Tag, Value>& get() {
            return *this;
        }

        void set(Tag tag_, Value value_) {
            this->tag = tag_;
            this->value = value_;
        }

        bool is(Tag tag_) {
            return this->tag == tag_;
        }

        Tag tag;
        Value value;
    };
}
