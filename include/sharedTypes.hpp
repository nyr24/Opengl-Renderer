#pragma once
#include <chrono>

namespace my_gl {
    using Duration_sec = std::chrono::duration<float, std::ratio<1>>;
    using Timepoint_sec = std::chrono::time_point<std::chrono::steady_clock, Duration_sec>;
}
