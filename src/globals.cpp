#include <math.h>
#include "globals.hpp"
#include "camera.hpp"

namespace my_gl {
    namespace globals {
        Camera camera({ 0.0f, 0.0f, 3.0f }, {0.0f, 1.0f, 0.0f});
        float                           delta_time{0.5f};
        my_gl::math::Vec4<float>        light_pos{0.0f, 1.0f, 1.0f, 1.0f};
    }
}
