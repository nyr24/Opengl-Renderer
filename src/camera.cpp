#include "camera.hpp"
#include "vec.hpp"
#include "matrix.hpp"

namespace my_gl {
    Camera::Camera(my_gl_math::Vec3<float>&& pos, my_gl_math::Vec3<float>&& dir, my_gl_math::Vec3<float>&& up)
        : camera_pos{ std::move(pos) }
        , camera_front{ std::move(dir) }
        , camera_up{ std::move(up) }
        , world_up{ std::move(up) }
    {}

    Camera::Camera(const my_gl_math::Vec3<float>& pos, const my_gl_math::Vec3<float>& dir, const my_gl_math::Vec3<float>& up)
        : camera_pos{ pos }
        , camera_front{ dir }
        , camera_up{ std::move(up) }
        , world_up{ up }
    {}

    my_gl_math::Matrix44<float> Camera::get_view_mat() {
        return my_gl_math::Matrix44<float>::look_at(camera_pos, camera_pos + camera_front, camera_up);
    }

    void Camera::process_keyboard_input(Camera_movement dir) {
        float velocity{ speed * globals::delta_time };

        switch(dir) {
        case Camera_movement::FORWARD:
            camera_pos += camera_front * velocity;
            break;
        case my_gl::Camera_movement::BACKWARD:
            camera_pos -= camera_front * velocity;
            break;
        case my_gl::Camera_movement::RIGHT:
            camera_pos += camera_right * velocity;
            break;
        case my_gl::Camera_movement::LEFT:
            camera_pos -= camera_right * velocity;
            break;
        }
    }

    void Camera::process_mouse_input(float mouse_curr_x, float mouse_curr_y, bool constrain_pitch) {
        if (mouse_is_first_event) {
            mouse_last_x = mouse_curr_x;
            mouse_last_y = mouse_curr_y;
            mouse_is_first_event = false;
        }

        float x_offset{ mouse_curr_x - mouse_last_x };
        float y_offset{ mouse_last_y - mouse_curr_y };
        mouse_last_x = mouse_curr_x;
        mouse_last_y = mouse_curr_y;

        x_offset *= sensivity;
        y_offset *= sensivity;

        yaw += x_offset;
        pitch += y_offset;

        if (constrain_pitch) {
            pitch = std::clamp(pitch, -89.0f, 89.0f);
        }

        update();
    }

    void Camera::process_scroll_input(float scroll_x_offset, float scroll_y_offset) {
        fov -= scroll_y_offset;
        fov = std::clamp(fov, 1.0f, 85.0f);
    }

    void Camera::update() {
        camera_front[0] = std::cos(my_gl_math::Global::degToRad(yaw)) *
            std::cos(my_gl_math::Global::degToRad(pitch));
        camera_front[1] = std::sin(my_gl_math::Global::degToRad(pitch));
        camera_front[2] = std::sin(my_gl_math::Global::degToRad(yaw)) *
            std::cos(my_gl_math::Global::degToRad(pitch));

        camera_front.normalize_inplace();
        // front and world_up already normalized here
        camera_right = camera_front.cross(world_up);
        camera_right.normalize_inplace();
        camera_up = camera_front.cross(camera_right).normalize_inplace();
    }
}
