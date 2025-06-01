#include "vec.hpp"
#include "matrix.hpp"

namespace my_gl {
template<std::floating_point T>
struct Physics {
public:
    Physics(
        math::Vec3<T>&& velocity,
        math::Vec3<T>&& acceleration,
        float           mass,
        math::Vec3<T>&& start_val = {0.0f, 0.0f, 0.0f}
    )
        : _mat{ math::Matrix44<T>::translation(start_val)}
        , _curr_val{ std::move(start_val) }
        , _velocity{ std::move(velocity) }
        , _acceleration{ std::move(acceleration) }
        , _mass{ mass }
    {}

    math::Matrix44<T>& update(float delta_time) {
        _velocity += _acceleration * delta_time;
        _curr_val += _velocity * delta_time;
        _mat.translate(_curr_val);
        return _mat;
    }

private:
    math::Matrix44<T>               _mat;
    my_gl::math::Vec3<T>            _curr_val;
public:
    my_gl::math::Vec3<T>            _velocity;
    my_gl::math::Vec3<T>            _acceleration;
    float                           _mass;
};
}
