#pragma once
#include <array>
#include <concepts>
#include <cassert>
#include <math.h>
#include <unordered_map>
#include "matrix.hpp"

namespace my_gl {
    enum ANIMATION_TYPE {
        TRANSLATE,
        ROTATE,
        ROTATE3d,
        SCALE
    };

    enum BEZIER_CURVE_TYPE {
        LINEAR,
        EASE_IN,
        EASE_OUT,
        EASE_IN_OUT,
        CURVE_COUNT
    };

    struct Bezier_curve_point_values {
        my_gl_math::Vec4<float> x_vals;
        my_gl_math::Vec4<float> y_vals;
    };

// for easier array initalization
    using Points = Bezier_curve_point_values;

    const std::array<Points, CURVE_COUNT> predefined_bezier_values = {
        Points{
            .x_vals = { 0.0f, 0.3f, 0.6f, 1.0f },
            .y_vals = { 0.0f, 0.3f, 0.6f, 1.0f }
        },
        Points{
            .x_vals = { 0.0f, 0.4f, 0.7f, 1.0f },
            .y_vals = { 0.0f, 0.0f, 0.45f, 1.0f }
        },
        Points{
            .x_vals = { 0.0f, 0.3f, 0.6f, 1.0f },
            .y_vals = { 0.0f, 0.55f, 1.0f, 1.0f }
        },
        Points{
            .x_vals = { 0.0f, 0.4f, 0.6f, 1.0f },
            .y_vals = { 0.0f, 0.0f, 1.0f, 1.0f }
        }
    };

    template<std::floating_point T = float>
    class Bezier_curve {
    public:
        Bezier_curve(
            const Points& init_values,
            BEZIER_CURVE_TYPE type
        )
            : _points{ init_values }
            , _mat{ my_gl_math::Matrix44<float>::bezier_cubic_mat() }
            , _type{ type }
        {}

    // get current time from curve
    // maps linear 0 to 1 range to bezier curve 0 to 1 range
        T update(T time_from_0to1) const {
            my_gl_math::Vec4<float> mon_basis_cubic{
                my_gl_math::Global::monomial_basis_cube(time_from_0to1) 
            };

            my_gl_math::Vec4<float> coefs_for_points{ _mat * mon_basis_cubic };

            my_gl_math::VecBase<T, 2> curr_val_vec = {
                _points.x_vals.dot(coefs_for_points),
                _points.y_vals.dot(coefs_for_points)
            };

            my_gl_math::VecBase<T, 2> difference_vec = { curr_val_vec - _init_dist_vec };

        // getting 1 at the end, and 0 at the start
        // signifying animation duration progress
            return difference_vec.length() - _init_dist_length;
        }

        const Points&     get_points() const { return _points; }
        BEZIER_CURVE_TYPE get_type() const { return _type; }

    private:
        const my_gl_math::Matrix44<float>    _mat;
    // x and y values of points a stored inside separate vectors to efficiently perform some operations
        const my_gl_math::VecBase<T, 2>      _init_dist_vec{ 1.0f, 1.0f };
        Points                               _points;
        BEZIER_CURVE_TYPE                    _type;
        static constexpr T                   _init_dist_length{ 1.0f };
    };

    template<std::floating_point VAL_TYPE>
    class Animation {
    public:
        Animation(
            ANIMATION_TYPE anim_type,
            my_gl_math::Vec3<VAL_TYPE>&& start_val,
            my_gl_math::Vec3<VAL_TYPE>&& end_val, 
            my_gl_math::Vec3<VAL_TYPE>&& delta_val,
            bool loop
        ) 
            : _anim_type{ anim_type }
            , _start_val{ std::move(start_val) }
            , _end_val{ std::move(end_val) }
            , _delta_val{ delta_val }
            , _curr_val{ _start_val }
            , _loop{ loop }
        {

            switch (anim_type) {
            case ANIMATION_TYPE::TRANSLATE:
                _mat = my_gl_math::Matrix44<VAL_TYPE>::translation(_curr_val);
                break;
            case ANIMATION_TYPE::SCALE:
                _mat = my_gl_math::Matrix44<VAL_TYPE>::scaling(_curr_val);
                break;
            case ANIMATION_TYPE::ROTATE3d:
                _mat = my_gl_math::Matrix44<VAL_TYPE>::rotation3d(_curr_val);
                break;
            case ANIMATION_TYPE::ROTATE:
                assert(true && "Can't use this ctor for rotate type animation"); 
                break;
            }
        }

        // ctor with duration
        // only supports predefined bezier values, dependent on passed 'bezier_type' 
        Animation(
            ANIMATION_TYPE anim_type,
            float duration,
            my_gl_math::Vec3<VAL_TYPE>&& start_val,
            my_gl_math::Vec3<VAL_TYPE>&& end_val, 
            BEZIER_CURVE_TYPE bezier_type = LINEAR
            //bool loop = false
        ) 
            : _anim_type{ anim_type }
            , _start_val{ std::move(start_val) }
            , _end_val{ std::move(end_val) }
            , _curr_val{ _start_val }
            , _duration{ duration }
            // loop option disabled currently because of time control issues
            , _loop{ false }
        {

            switch (anim_type) {
            case ANIMATION_TYPE::TRANSLATE:
                _mat = my_gl_math::Matrix44<VAL_TYPE>::translation(_curr_val);
                break;
            case ANIMATION_TYPE::SCALE:
                _mat = my_gl_math::Matrix44<VAL_TYPE>::scaling(_curr_val);
                break;
            case ANIMATION_TYPE::ROTATE3d:
                _mat = my_gl_math::Matrix44<VAL_TYPE>::rotation3d(_curr_val);
                break;
            case ANIMATION_TYPE::ROTATE:
                assert(true && "Can't use this ctor for rotate type animation"); 
                break;
            }

            switch (bezier_type) {
            case BEZIER_CURVE_TYPE::LINEAR:
                _bezier_curve = {
                    predefined_bezier_values[LINEAR],
                    LINEAR
                };
                break;
            case BEZIER_CURVE_TYPE::EASE_IN:
                _bezier_curve = {
                    predefined_bezier_values[EASE_IN],
                    EASE_IN
                };
                break;
            case BEZIER_CURVE_TYPE::EASE_OUT:
                _bezier_curve = {
                    predefined_bezier_values[EASE_OUT],
                    EASE_IN_OUT
                };
                break;
            case BEZIER_CURVE_TYPE::EASE_IN_OUT:
                _bezier_curve = {
                    predefined_bezier_values[EASE_IN_OUT],
                    EASE_IN_OUT
                };
                break;
            default:
                assert(true && "invalid initialization bezier curve type"); 
                break;
            }
        } 

        // constructor for rotation around single axis
        Animation(
            VAL_TYPE                 start_val,
            VAL_TYPE                 end_val, 
            VAL_TYPE                 delta_val,
            my_gl_math::Global::AXIS axis,
            bool loop
        ) 
            : _anim_type{ ANIMATION_TYPE::ROTATE }
            , _axis{ axis }
            , _loop{ loop }
        {
            switch (_axis) {
            case my_gl_math::Global::X:
                _start_val[0] = start_val;
                _end_val[0] = end_val;
                _delta_val[0] = delta_val;
                _curr_val = _start_val;
                _mat = my_gl_math::Matrix44<VAL_TYPE>::rotation(_curr_val.x(), _axis);
                break;
            case my_gl_math::Global::Y:
                _start_val[1] = start_val;
                _end_val[1] = end_val;
                _delta_val[1] = delta_val;
                _curr_val = _start_val;
                _mat = my_gl_math::Matrix44<VAL_TYPE>::rotation(_curr_val.y(), _axis);
                break;
            case my_gl_math::Global::Z:
                _start_val[2] = start_val;
                _end_val[2] = end_val;
                _delta_val[2] = delta_val;
                _curr_val = _start_val;
                _mat = my_gl_math::Matrix44<VAL_TYPE>::rotation(_curr_val.z(), _axis);
                break;
            }
        }

        my_gl_math::Matrix44<VAL_TYPE>& update() {
            if (_is_ended) {
                return _mat;
            }
            
            for (int i = 0; i < 3; ++i) {
                if (_delta_val[i] >= static_cast<VAL_TYPE>(0)) {
                    _curr_val[i] = std::min(_curr_val[i] + _delta_val[i], _end_val[i]);
                }
                else {
                    _curr_val[i] = std::max(_curr_val[i] + _delta_val[i], _end_val[i]);
                }
            }
            // check if end
            if (_curr_val.cmp(_end_val)) {
                if (_loop) {
                    // invert animation if loop
                    _delta_val *= static_cast<VAL_TYPE>(-1.0);
                    std::swap(_start_val, _end_val);

                    update_matrix();
                    return _mat;
                }
                else {
                    _is_ended = true;
                    return _mat;
                }
            }
            else {
                update_matrix();
                return _mat;
            }
        }

        // based on curr time & duration & bezier curve 
        my_gl_math::Matrix44<VAL_TYPE>& update(float curr_time) {
            if (!_loop && (curr_time > _duration)) {
                return _mat;
            }
            
            float linear_0to1{ (fmod(curr_time, _duration)) / _duration };
            float bezier_0to1{ _bezier_curve.update(linear_0to1) };
            _curr_val = my_gl_math::Global::lerp<my_gl_math::Vec3<VAL_TYPE>, VAL_TYPE>(_start_val, _end_val, bezier_0to1);
            update_matrix();
            return _mat;
        }


    private:
        my_gl_math::Matrix44<VAL_TYPE>      _mat;
        Bezier_curve<VAL_TYPE>              _bezier_curve;
        my_gl_math::Vec3<VAL_TYPE>          _start_val;
        my_gl_math::Vec3<VAL_TYPE>          _end_val;
        my_gl_math::Vec3<VAL_TYPE>          _delta_val;
        my_gl_math::Vec3<VAL_TYPE>          _curr_val;
        float                               _duration;
        ANIMATION_TYPE                      _anim_type;
        // only for rotation
        my_gl_math::Global::AXIS            _axis{ my_gl_math::Global::Z };
        bool                                _loop{ false };
        bool                                _is_ended{ false };

        void update_matrix() {
            switch (_anim_type) {
            case my_gl::TRANSLATE:
                _mat.translate(_curr_val);
                break;
            case my_gl::ROTATE3d:
                _mat.rotate3d(_curr_val);
                break;
            case my_gl::SCALE:
                _mat.scale(_curr_val);
                break;
            case my_gl::ROTATE:
                if (_axis == my_gl_math::Global::X)
                    _mat.rotate(_curr_val.x(), _axis);
                else if (_axis == my_gl_math::Global::Y)
                    _mat.rotate(_curr_val.y(), _axis);
                else
                    _mat.rotate(_curr_val.z(), _axis);
                break;
            }
        }
    };
}