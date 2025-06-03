#include <iostream>
#include "window.hpp"

my_gl::Window::Window(int32_t width, int32_t height, const char* name, GLFWmonitor* monitor, GLFWwindow* share)
    : ptr{ glfwCreateWindow(width, height, name, monitor, share) }
    , width{ width }
    , height{ height }
{
    if (ptr == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
}

my_gl::Window::Window(my_gl::Window&& rhs) noexcept
    : ptr{ rhs.ptr }
    , width{ rhs.width }
    , height{ rhs.height }
{
    rhs.ptr = nullptr;
    rhs.width = 0;
    rhs.height = 0;
}

my_gl::Window& my_gl::Window::operator=(my_gl::Window&& rhs) noexcept
{
    ptr = rhs.ptr;
    width = rhs.width;
    height = rhs.height;

    rhs.ptr = nullptr;
    rhs.width = 0;
    rhs.height = 0;

    return *this;
}

my_gl::Window::~Window() {
    glfwTerminate();
}
