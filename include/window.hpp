#pragma once
#include "GLFW/glfw3.h"

namespace my_gl {
    class Window {
        public:
            Window(int32_t width, int32_t height, const char* name, GLFWmonitor* monitor, GLFWwindow* share);
            Window(Window&& w) noexcept;
            Window& operator=(Window&& w) noexcept;
            Window(const Window& w) = delete;
            Window& operator=(const Window& w) = delete;
            ~Window();

        public:
            GLFWwindow*     ptr;
            int32_t         width;
            int32_t         height;
    };
}
