#include <math.h>
#include "globals.hpp"
#include "camera.hpp"

namespace my_gl {
    // using default constructor;
    Camera camera{};

    namespace globals {
        float                       delta_time{0.5f};
        my_gl_math::Vec3<float>     light_pos{1.0f, 0.9f, 0.85f};
    }
}
