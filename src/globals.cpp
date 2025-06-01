#include "globals.hpp"
#include "camera.hpp"

namespace my_gl {
    namespace globals {
        Camera camera({ 0.0f, 0.0f, 9.0f }, {0.0f, 1.0f, 0.0f});
        float delta_time{0.5f};

        Light light{
            .position = {0.0f, 0.0f, 3.5f},
            .ambient = {0.8f, 0.8f, 0.8f},
            .diffuse = {0.5f, 0.5f, 0.5f},
            .specular = {1.0f, 1.0f, 1.0f},
        };
    }
}
