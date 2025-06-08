#include "globals.hpp"
#include "camera.hpp"

namespace my_gl {
    namespace globals {
        Camera camera({ 0.0f, 0.0f, 15.0f }, {0.0f, 1.0f, 0.0f});
        float delta_time{0.5f};

        Light light{
            .position = {0.0f, 0.0f, 7.0f},
            .ambient = {1.0f, 1.0f, 1.0f},
            .diffuse = {1.0f, 1.0f, 1.0f},
            .specular = {1.0f, 1.0f, 1.0f},
        };
    }
}
