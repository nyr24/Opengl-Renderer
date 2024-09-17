#include <iostream>
#include "window.h"

my_gl::Window::Window(int width, int height, const char* name, GLFWmonitor* monitor, GLFWwindow* share) 
    : m_window{ glfwCreateWindow(width, height, name, monitor, share) }
    , m_width{ width }
    , m_height{ height } 
{
    if (m_window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(m_window);
}

my_gl::Window::~Window() {
    glfwTerminate();
}

GLFWwindow* my_gl::Window::ptr_raw() const { return m_window; }
int my_gl::Window::width() const { return m_width; }
int my_gl::Window::height() const { return m_height; }