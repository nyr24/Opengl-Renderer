#include "animation.hpp"

// Rotation
/* my_gl::Rotation::Rotation(float start_angle_deg, float end_angle_deg, float delta_deg, my_gl_math::Global::AXIS axis)
    : _start_angle{ start_angle_deg }
    , _end_angle{ end_angle_deg }
    , _delta{ delta_deg }
    , _curr_angle{ start_angle_deg }
    , _axis{ axis }
    , _mat{ my_gl_math::Matrix44<float>::rotation(start_angle_deg, axis) }
{}

bool my_gl::Rotation::check_borders() const {
    return (_curr_angle >= _start_angle) || (_curr_angle <= _end_angle);
}

void my_gl::Rotation::update() {
    _curr_angle += _delta;
    if (!check_borders()) {
        _delta *= -1.0f;
    }

    _mat.rotate(_curr_angle, _axis);
}

const float* my_gl::Rotation::data() const {
    return _mat.data();
}

const my_gl_math::Matrix44<float>& my_gl::Rotation::mat() const {
    return _mat;
} */

// Translation
my_gl::Translation::Translation(
    const my_gl_math::Vec3<float>&& from_vec,
    const my_gl_math::Vec3<float>&& to_vec, 
    const my_gl_math::Vec3<float>&& delta,
    bool loop
) 
    : _from_vec{ std::move(from_vec) }
    , _to_vec{ std::move(to_vec) }
    , _curr_val{ _from_vec }
    , _delta{ std::move(delta) }
    , _mat{ my_gl_math::Matrix44<float>::identity() }
    , _loop{ loop }
{}

const my_gl_math::Matrix44<float>* const my_gl::Translation::update() {
    for (int i = 0; i < 3; ++i) {
        if (_delta[i] > 0) {
            _curr_val[i] = std::min(_curr_val[i] + _delta[i], _to_vec[i]);
        }
        else {
            _curr_val[i] = std::max(_curr_val[i] + _delta[i], _to_vec[i]);
        }
    }
    // check if end
    if (_curr_val.is_eq(_to_vec)) {
        if (_loop) {
            // invert animation if loop
            _delta *= -1.0f;
            std::swap(_from_vec, _to_vec);

            _mat.translate(_curr_val);
            return &_mat;
        }
        else {
            _is_ended = true;
            return nullptr;
        }
    }
    else {
        _mat.translate(_curr_val);
        return &_mat;
    }
}

/* bool my_gl::Translation::check_borders() const {
    return (_curr_val >= _start_val) || (_curr_val <= _end_val);
} */


/* const float* my_gl::Translation::data() const {
    return _mat.data();
}

const my_gl_math::Matrix44<float>& my_gl::Translation::mat() const {
    return _mat;
} */