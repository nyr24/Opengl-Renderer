#pragma once

#include <span>
#include <cstdint>
#include "vec.hpp"

namespace my_gl {

namespace math {
    template<std::floating_point T>
    class Matrix44;
} // math

namespace meshes {

struct Boundaries {
    my_gl::math::Vec3<float> ltn;
    my_gl::math::Vec3<float> ltf;
    my_gl::math::Vec3<float> rtn;
    my_gl::math::Vec3<float> rtf;
    my_gl::math::Vec3<float> lbn;
    my_gl::math::Vec3<float> lbf;
    my_gl::math::Vec3<float> rbn;
    my_gl::math::Vec3<float> rbf;
};

// this is just a view
struct Mesh {
    std::span<float>            vertices;
    std::span<uint16_t>         indices;
    const Boundaries*           boundaries;

    Boundaries  transform_boundaries(const my_gl::math::Matrix44<float>& mat) const;
};

extern Mesh get_cube_mesh();

} // meshes
} // my_gl
