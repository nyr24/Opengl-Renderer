#pragma once

namespace my_gl  {
    class Camera;

    namespace globals {
        struct WindowProps {
            static constexpr int width          { 800 };
            static constexpr int height         { 600 };
        };

        extern WindowProps                  window_props;
        extern float                        delta_time;
    }

    extern Camera camera;
}
