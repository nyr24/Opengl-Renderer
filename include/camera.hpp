#pragma once

#include "globals.hpp"
#include "math.hpp"
#include "matrix.hpp"
#include "vec.hpp"

namespace my_gl {
    enum class Camera_movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    class Camera {
    public:
        my_gl_math::Vec3<float> camera_pos          { 0.0f, 0.0f, 3.0f };
        my_gl_math::Vec3<float> camera_front        { 0.0f, 0.0f, -1.0f };
        my_gl_math::Vec3<float> camera_up           { 0.0f, 1.0f, 0.0f };
        my_gl_math::Vec3<float> camera_right        { 1.0f, 0.0f, 0.0f };
        my_gl_math::Vec3<float> world_up            { 0.0f, 1.0f, 0.0f };
        float                   mouse_last_x        { static_cast<float>(globals::window_props.width) / 2.0f };
        float                   mouse_last_y        { static_cast<float>(globals::window_props.height) / 2.0f };
        float                   pitch               { 0.0f };
        float                   yaw                 { -90.0f };
        float                   fov                 { 45.0f };
        static constexpr float  speed               { 2.5f };
        static constexpr float  sensivity           { 0.1f };
        static constexpr float  aspect              { static_cast<float>(globals::window_props.width) / globals::window_props.height };
        bool                    mouse_is_first_event{ true };

        Camera() = default;
        Camera(my_gl_math::Vec3<float>&& pos, my_gl_math::Vec3<float>&& dir, my_gl_math::Vec3<float>&& up);
        Camera(const my_gl_math::Vec3<float>& pos, const my_gl_math::Vec3<float>& dir, const my_gl_math::Vec3<float>& up);

        my_gl_math::Matrix44<float>     get_view_mat();
        void                            process_keyboard_input(Camera_movement dir);
        void                            process_mouse_input(float mouse_curr_x, float mouse_curr_y, bool constain_pitch = true);
        void                            process_scroll_input(float scroll_x_offset, float scroll_y_offset);

    private:
        void update();
    };
}
