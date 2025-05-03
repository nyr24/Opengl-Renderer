#include <math.h>
#include "globals.hpp"
#include "camera.hpp"

namespace my_gl {
    namespace globals {
        // using default constructor;
        Camera camera{};
        float                           delta_time{0.5f};
        my_gl::math::Vec3<float>        light_pos{1.0f, 0.9f, 0.85f};
    }
}
