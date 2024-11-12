#pragma once
#include <array>
#include <concepts>
#include <cassert>
#include <math.h>
#include <unordered_map>
#include <chrono>
// TEMP
#include <iostream>
#include "matrix.hpp"
#include "shared_types.hpp"

namespace my_gl {
    enum Animation_type {
        TRANSLATE,
        ROTATE,
        ROTATE3d,
        SCALE
    };

    enum Bezier_curve_type {
        LINEAR,
        EASE_IN,
        EASE_OUT,
        EASE_IN_OUT,
        CURVE_COUNT
    };

    enum Loop_type {
        NONE,
        DEFAULT,
        INVERT
    };

    //template<std::floating_point Val_type>
    struct Bezier_curve_point_values {
        my_gl_math::Vec4<float> x_vals;
        my_gl_math::Vec4<float> y_vals;
    };

    // for easier array initalization
    // template<std::floating_point Val_type = float>
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
        Bezier_curve() = default;
        Bezier_curve(
            const Points&       init_values,
            Bezier_curve_type   type
        )
            : _points{ init_values }
            , _mat{ my_gl_math::Matrix44<float>::bezier_cubic_mat() }
            , _type{ type }
        {}
        Bezier_curve(
            Points&&         init_values,
            Bezier_curve_type   type
        )
            : _points{ std::move(init_values) }
            , _mat{ my_gl_math::Matrix44<float>::bezier_cubic_mat() }
            , _type{ type }
        {}
        Bezier_curve(const Bezier_curve<T>& rhs) = default;
        Bezier_curve(Bezier_curve<T>&& rhs) = default;

        Bezier_curve<T>& operator=(const Bezier_curve<T>& rhs) = default;
        Bezier_curve<T>& operator=(Bezier_curve<T>&& rhs) = default;

        // get current time from curve
        // maps linear 0 to 1 range to bezier curve 0 to 1 range
        T update(T time_from_0to1) const {
            my_gl_math::Vec4<T> mon_basis_cubic{
                my_gl_math::Global::monomial_basis_cube(time_from_0to1) 
            };

            my_gl_math::Vec4<T> coefs_for_points{ _mat * mon_basis_cubic };

            my_gl_math::VecBase<T, 2> curr_val = {
                _points.x_vals.dot(coefs_for_points),
                _points.y_vals.dot(coefs_for_points)
            };

            my_gl_math::VecBase<T, 2> distance = { curr_val - my_gl_math::VecBase<T, 2>(1.0f) };

            // getting 1 at the end, and 0 at the start
            // signifying animation duration progress
            return 1.0f - distance.length();
        }

        const Points&     get_points() const { return _points; }
        Bezier_curve_type get_type() const { return _type; }

    private:
        my_gl_math::Matrix44<float>                 _mat;
        // x and y values of points a stored inside separate vectors to efficiently perform math operations
        Points                                      _points{ predefined_bezier_values[LINEAR] };
        Bezier_curve_type                           _type{ LINEAR };
    };


    template<std::floating_point Val_type>
    class Animation {
    public:
        // main ctor
        // only supports predefined bezier values, dependent on passed 'bezier_type' 
        Animation(
            Animation_type                  anim_type,
            float                           duration,
            my_gl_math::Vec3<Val_type>&&    start_val,
            my_gl_math::Vec3<Val_type>&&    end_val, 
            Bezier_curve_type               bezier_type = LINEAR,
            Loop_type                       loop = Loop_type::NONE
        ) 
            : _anim_type{ anim_type }
            , _start_val{ std::move(start_val) }
            , _end_val{ std::move(end_val) }
            //, _curr_val{ _start_val }
            , _duration{ duration }
            , _loop{ loop }
        {

            switch (anim_type) {
            case Animation_type::TRANSLATE:
                _mat = my_gl_math::Matrix44<Val_type>::translation(_start_val);
                break;
            case Animation_type::SCALE:
                _mat = my_gl_math::Matrix44<Val_type>::scaling(_start_val);
                break;
            case Animation_type::ROTATE3d:
                _mat = my_gl_math::Matrix44<Val_type>::rotation3d(_start_val);
                break;
            case Animation_type::ROTATE:
                assert(true && "Can't use this ctor for rotate type animation"); 
                break;
            }

            set_bezier_type(bezier_type);
        } 

        // constructor for rotation around single axis
        Animation(
            Val_type                 start_val,
            Val_type                 end_val,
            float                    duration,
            my_gl_math::Global::AXIS axis,
            Bezier_curve_type        bezier_type = LINEAR,
            Loop_type                loop = Loop_type::NONE
        ) 
            : _anim_type{ Animation_type::ROTATE }
            , _axis{ axis }
            , _duration{ duration }
            , _loop{ loop }
        {
            switch (_axis) {
            case my_gl_math::Global::X:
                _start_val[0] = start_val;
                _end_val[0] = end_val;
                _mat = my_gl_math::Matrix44<Val_type>::rotation(_start_val.x(), _axis);
                break;
            case my_gl_math::Global::Y:
                _start_val[1] = start_val;
                _end_val[1] = end_val;
                _mat = my_gl_math::Matrix44<Val_type>::rotation(_start_val.y(), _axis);
                break;
            case my_gl_math::Global::Z:
                _start_val[2] = start_val;
                _end_val[2] = end_val;
                _mat = my_gl_math::Matrix44<Val_type>::rotation(_start_val.z(), _axis);
                break;
            }

            set_bezier_type(bezier_type);
        }

        // updates inner matrix based on current time & interpolated value & choosen bezier curve type
        my_gl_math::Matrix44<Val_type>& update() {
            if (_loop == Loop_type::NONE && _is_ended) {
                return _mat;
            }
            if (!_is_started) {
                _start_time = std::chrono::steady_clock::now();
                _curr_time = _start_time;
                _is_started = true;
            }

            Duration_sec passed_time{ _curr_time - _start_time }; 
            
            float linear_0to1{ passed_time / _duration };
            float bezier_0to1{ _bezier_curve.update(linear_0to1) };

            auto _curr_val{ 
                my_gl_math::Global::lerp(_start_val, _end_val, bezier_0to1) 
            };

            update_matrix(_curr_val);
            return _mat;
        }

        // should be called at the end of the current frame
        // to properly update curr animation time
        void update_time(Duration_sec frame_time) {
            if (_is_reversed) {
                frame_time *= -1.0;
            }

            _curr_time += frame_time;

            if (_curr_time >= (_start_time + _duration)) {
                if (_loop == Loop_type::NONE) {
                    _is_ended = true;
                }
                else if (_loop == Loop_type::DEFAULT) {
                    _curr_time = _start_time;
                }
                else {
                    _is_reversed = !_is_reversed;
                }
            }
            else if (_curr_time <= _start_time && _loop == Loop_type::INVERT) {
                _is_reversed = !_is_reversed;
            }
        }

    private:
        my_gl_math::Matrix44<Val_type>      _mat;
        Bezier_curve<float>                 _bezier_curve;
        my_gl_math::Vec3<Val_type>          _start_val;
        my_gl_math::Vec3<Val_type>          _end_val;
        // make sence only with pause / play system
        //my_gl_math::Vec3<Val_type>          _curr_val;
        Timepoint_sec                       _start_time;
        Timepoint_sec                       _curr_time;
        Duration_sec                        _duration;
        // only for single-axis rotation
        my_gl_math::Global::AXIS            _axis{ my_gl_math::Global::Z };
        Animation_type                      _anim_type;
        Loop_type                           _loop{ Loop_type::NONE };
        bool                                _is_started{ false };
        bool                                _is_ended{ false };
        bool                                _is_reversed{ false };
        //bool                                _is_paused{ false };

        // input can be Vector or Scalar (single axis rotation)
        void update_matrix(const auto& _curr_val) {
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

        void set_bezier_type(Bezier_curve_type bezier_type) {
            switch (bezier_type) {
            case Bezier_curve_type::LINEAR:
                _bezier_curve = {
                    predefined_bezier_values[LINEAR],
                    LINEAR
                };
                break;
            case Bezier_curve_type::EASE_IN:
                _bezier_curve = {
                    predefined_bezier_values[EASE_IN],
                    EASE_IN
                };
                break;
            case Bezier_curve_type::EASE_OUT:
                _bezier_curve = {
                    predefined_bezier_values[EASE_OUT],
                    EASE_IN_OUT
                };
                break;
            case Bezier_curve_type::EASE_IN_OUT:
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
    };
}