#include <math.h>
#include "globals.hpp"
#include "camera.hpp"

namespace my_gl {
    namespace globals {
        Camera camera({ 0.0f, 0.0f, 3.0f }, {0.0f, 1.0f, 0.0f});
        float delta_time{0.5f};

        Light light{
            .position = {1.1f, 1.1f, 1.5f},
            .ambient = {0.2f, 0.2f, 0.2f},
            .diffuse = {0.5f, 0.5f, 0.5f},
            .specular = {1.0f, 1.0f, 1.0f},
        };
    }
}
