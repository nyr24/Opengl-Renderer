#pragma once
#include "GLFW/glfw3.h"

namespace my_gl {
    class Window {
        public:
            Window(int width, int height, const char* name, GLFWmonitor* monitor, GLFWwindow* share);
            ~Window();

            GLFWwindow* ptr_raw() const;
            int width() const;
            int height() const;

        private:
            GLFWwindow* m_window;
            int m_width;
            int m_height;
    };
}