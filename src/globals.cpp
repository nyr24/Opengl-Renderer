#include <math.h>
#include "globals.hpp"

namespace globals {
    float delta_time{0.5f};

    CameraProps camera_props{
        .pos{ 0.0f, 0.0f, 3.0f },
        .front{ 0.0f, 0.0f, -1.0f },
        .up{ 0.0f, 1.0f, 0.0f },
        .pitch = 0.0f,
        .yaw = -90.0f,
        .speed = 2.5f * delta_time,
        .fov = 45.0f,
        .aspect = static_cast<float>(window_props.width) / window_props.height
    };

    MouseProps mouse_props {
        .last_x = window_props.width / 2.0f,
        .last_y = window_props.height / 2.0f,
        .is_first_mouse_event = true
    };
}
