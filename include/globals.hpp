#pragma once

#include "vec.hpp"

namespace my_gl  {
    class Camera;

    namespace globals {
        struct WindowProps {
            static constexpr int width          { 800 };
            static constexpr int height         { 600 };
        };

        extern WindowProps                  window_props;
        extern float                        delta_time;
        extern math::Vec4<float>            light_pos;
        extern Camera camera;
    }

}
