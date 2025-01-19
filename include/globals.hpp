#pragma once
#include "math.hpp"
#include "vec.hpp"

namespace globals {
    struct WindowProps {
        static constexpr int width{ 800 };
        static constexpr int height{ 600 };
    };

    struct CameraProps {
        my_gl_math::Vec3<float>         pos;
        my_gl_math::Vec3<float>         front;
        const my_gl_math::Vec3<float>   up;
        float                           pitch;
        float                           yaw;
        float                           speed;
        float                           fov;
        float                           aspect;
    };

    struct MouseProps {
        float                           last_x;
        float                           last_y;
        bool                            is_first_mouse_event;
        static constexpr float          sensivity{ 0.1f };
    };

    extern WindowProps                  window_props;
    extern CameraProps                  camera_props;
    extern MouseProps                   mouse_props;
    extern float                        delta_time;
}
