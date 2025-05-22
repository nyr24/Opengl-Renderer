#pragma once

#include "vec.hpp"

namespace my_gl  {
    class Camera;

    struct Light {
        my_gl::math::Vec3<float> position;
        my_gl::math::Vec3<float> ambient;
        my_gl::math::Vec3<float> diffuse;
        my_gl::math::Vec3<float> specular;
    };

    namespace globals {
        struct WindowProps {
            static constexpr int width          { 800 };
            static constexpr int height         { 600 };
        };

        extern WindowProps                  window_props;
        extern float                        delta_time;
        extern math::Vec4<float>            light_pos;
        extern Camera   camera;
        extern Light    light;
    }
}
