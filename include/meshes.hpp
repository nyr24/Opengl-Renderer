#pragma once

#include <vector>
#include <cstdint>

namespace my_gl {
    namespace meshes {
        struct Mesh {
            std::vector<float>      vertices;
            std::vector<uint16_t>   indices;
        };

        extern Mesh cube_mesh;
    }
}
