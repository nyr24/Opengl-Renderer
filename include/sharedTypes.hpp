#pragma once
#include <chrono>
#include <concepts>

namespace my_gl_math {
    template<std::floating_point T>
    class Matrix44;
}

namespace my_gl {
    using Duration_sec = std::chrono::duration<float, std::ratio<1>>;
    using Timepoint_sec = std::chrono::time_point<std::chrono::steady_clock, Duration_sec>;

    class IRenderable {
    public:
        virtual void render(const my_gl_math::Matrix44<float>& world_matrix, float time_0to1) const = 0;
        virtual void update_anims_time(Duration_sec frame_time) const = 0;
    };
}
