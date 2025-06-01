#pragma once
#include <array>
#include <concepts>
#include <cassert>
#include <chrono>
#include <variant>
#include "math.hpp"
#include "matrix.hpp"
#include "sharedTypes.hpp"
#include "vec.hpp"
#ifdef DEBUG
#include <iostream>
#endif

namespace my_gl {
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

    //template<std::floating_point T>
    struct Bezier_curve_point_values {
        math::Vec4<float> x_vals;
        math::Vec4<float> y_vals;
    };

    // for easier array initalization
    // template<std::floating_point T = float>
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
    struct Bezier_curve {
    public:
        Bezier_curve() = default;
        Bezier_curve(
            const Points&       init_values,
            Bezier_curve_type   type
        )
            : _mat{ math::Matrix44<float>::bezier_cubic_mat() }
            , _points{ init_values }
            , _type{ type }
        {}
        Bezier_curve(
            Points&&            init_values,
            Bezier_curve_type   type
        )
            : _mat{ math::Matrix44<float>::bezier_cubic_mat() }
            , _points{ std::move(init_values) }
            , _type{ type }
        {}
        Bezier_curve(const Bezier_curve<T>& rhs) = default;
        Bezier_curve(Bezier_curve<T>&& rhs) = default;

        Bezier_curve<T>& operator=(const Bezier_curve<T>& rhs) = default;
        Bezier_curve<T>& operator=(Bezier_curve<T>&& rhs) = default;

        // get current time from curve
        // maps linear 0 to 1 range to bezier curve 0 to 1 range
        T update(T time_from_0to1) const {
            math::Vec4<T> mon_basis_cubic{
                math::Global::monomial_basis_cube(time_from_0to1) 
            };

            math::Vec4<T> coefs_for_points{ _mat * mon_basis_cubic };

            math::VecBase<T, 2u> curr_val = {
                _points.x_vals.dot(coefs_for_points),
                _points.y_vals.dot(coefs_for_points)
            };

            // current distance between current and end point of the curve
            math::VecBase<T, 2u> curr_distance = { curr_val - _vec_end };

            // getting 1 at the end, and 0 at the start
            // signifying animation duration progress
            return (_max_distance - curr_distance.length()) * _unit_to_max_ratio;
        }

        math::Matrix44<T>                               _mat;
        // x and y values of points a stored inside separate vectors to efficiently perform math operations
        Points                                          _points{ predefined_bezier_values[LINEAR] };
        Bezier_curve_type                               _type{ LINEAR };
        // utility
        static inline const math::VecBase<T, 2u>         _vec_end{ T(1.0), T(1.0) };
        static inline const T                           _max_distance{ _vec_end.length() };
        static inline const T                           _unit_to_max_ratio{ T(1.0) / _max_distance };
    };

    template<std::floating_point T>
    struct AnimValue {
        std::variant<math::Vec3<T>, math::VecBase<T, 2u>, T> variant;

        template<typename SetVal>
        void set(SetVal&& value) {
            static_assert(sizeof(SetVal) == sizeof(math::Vec3<T>)
                    || sizeof(SetVal) == sizeof(math::VecBase<T, 2>)
                    || sizeof(SetVal) == sizeof(T), "Not a valid type to set");
            variant = std::move(value);
        }

        const math::Vec3<T>* get_vec3() const {
            return std::get_if<math::Vec3<T>>(&variant);
        }

        const math::VecBase<T, 2u>* get_vec2() const {
            return std::get_if<math::VecBase<T, 2u>>(&variant);
        }

        const T* get_scalar() const {
            return std::get_if<T>(&variant);
        }
    };

    template<std::floating_point T>
    struct Animation {
        static Animation<T> translation(
            float               duration,
            float               delay,
            math::Vec3<T>&&     start_val,
            math::Vec3<T>&&     end_val,
            Bezier_curve_type   bezier_type = LINEAR,
            Loop_type           loop = Loop_type::NONE
        )
        {
            return Animation<T>{
                ._bezier_curve{ Bezier_curve<T>{ predefined_bezier_values[bezier_type], bezier_type }},
                ._mat{ math::Matrix44<T>::translation(start_val) },
                ._start_val{ start_val },
                ._end_val{ end_val },
                ._duration{ Duration_sec{duration} },
                ._delay{ Duration_sec{delay} },
                ._anim_type = math::TransformationType::TRANSLATION,
                ._loop = loop,
                ._is_delay_passed = delay == 0.0f
            };
        }

        static Animation<T> scaling(
            float               duration,
            float               delay,
            math::Vec3<T>&&     start_val,
            math::Vec3<T>&&     end_val,
            Bezier_curve_type   bezier_type = LINEAR,
            Loop_type           loop = Loop_type::NONE
        )
        {
            return Animation<T>{
                ._bezier_curve{ Bezier_curve<T>{ predefined_bezier_values[bezier_type], bezier_type }},
                ._mat{ math::Matrix44<T>::scaling(start_val) },
                ._start_val{ start_val },
                ._end_val{ end_val },
                ._duration{ Duration_sec{duration} },
                ._delay{ Duration_sec{delay} },
                ._anim_type = math::TransformationType::SCALING,
                ._loop = loop,
                ._is_delay_passed = delay == 0.0f
            };
        }

        static Animation<T> rotation3d(
            float               duration,
            float               delay,
            math::Vec3<T>&&     start_val,
            math::Vec3<T>&&     end_val,
            Bezier_curve_type   bezier_type = LINEAR,
            Loop_type           loop = Loop_type::NONE
        )
        {
            return Animation<T>{
                ._bezier_curve{ Bezier_curve<T>{ predefined_bezier_values[bezier_type], bezier_type }},
                ._mat{ math::Matrix44<T>::rotation3d(start_val) },
                ._start_val{ start_val },
                ._end_val{ end_val },
                ._duration{ Duration_sec{duration} },
                ._delay{ Duration_sec{delay} },
                ._anim_type = math::TransformationType::ROTATION3d,
                ._loop = loop,
                ._is_delay_passed = delay == 0.0f
            };
        }

        static Animation<T> rotation_single_axis(
            float               duration,
            float               delay,
            T                   start_val,
            T                   end_val,
            math::Global::AXIS  axis,
            Bezier_curve_type   bezier_type = LINEAR,
            Loop_type           loop = Loop_type::NONE
        )
        {
            return Animation<T>{
                ._bezier_curve{ Bezier_curve<T>{ predefined_bezier_values[bezier_type], bezier_type }},
                ._mat{ math::Matrix44<T>::rotation(start_val, axis) },
                ._start_val{ start_val },
                ._end_val{ end_val },
                ._duration{ Duration_sec{duration} },
                ._delay{ Duration_sec{delay} },
                ._axis = axis,
                ._anim_type = math::TransformationType::ROTATION,
                ._loop = loop,
                ._is_delay_passed = delay == 0.0f
            };
        }

        static Animation<T> shear(
            float                   duration,
            float                   delay,
            math::VecBase<T, 2u>    start_val,
            math::VecBase<T, 2u>    end_val,
            math::Global::AXIS      axis,
            Bezier_curve_type       bezier_type = LINEAR,
            Loop_type               loop = Loop_type::NONE
        )
        {
            return Animation<T>{
                ._bezier_curve{ Bezier_curve<T>{ predefined_bezier_values[bezier_type], bezier_type }},
                ._mat{ math::Matrix44<T>::shearing(start_val, axis) },
                ._start_val{ start_val },
                ._end_val{ end_val },
                ._duration{ Duration_sec{duration} },
                ._delay{ Duration_sec{delay} },
                ._axis = axis,
                ._anim_type = math::TransformationType::SHEAR,
                ._loop = loop,
            };
        }

        // updates inner matrix based on current time & interpolated value & choosen bezier curve type
        math::Matrix44<T>& update() {
            if (_loop == Loop_type::NONE && _is_ended) {
                return _mat;
            }
            if (!_is_started) {
                _start_time = std::chrono::steady_clock::now();
                _curr_time = _start_time;
                _is_started = true;
            }

            // can't be bigger than duration, see update_time()
            Duration_sec passed_time{ _curr_time - _start_time };

            if (_delay.count() > 0.0f && !_is_delay_passed) {
                if (passed_time.count() >= _delay.count()) {
                    _start_time = std::chrono::steady_clock::now();
                    _curr_time = _start_time;
                    _is_delay_passed = true;
                }
                else {
                    return _mat;
                }
            }

            float linear_0to1{ passed_time / _duration };
            if (_bezier_curve._type != Bezier_curve_type::LINEAR) {
                linear_0to1 = _bezier_curve.update(linear_0to1);
            }

            switch (this->_anim_type) {
                case math::TransformationType::TRANSLATION: {
                    const math::Vec3<T>* start_unwrapped = _start_val.get_vec3();
                    const math::Vec3<T>* end_unwrapped = _end_val.get_vec3();
                    assert(start_unwrapped && end_unwrapped && "Vec3 expected from unwrapping");
                    math::Vec3<T> lerp_val = math::Global::lerp(*start_unwrapped, *end_unwrapped, linear_0to1);
                    _mat.translate(lerp_val);
                    return _mat;
                } break;
                case math::TransformationType::SCALING: {
                    const math::Vec3<T>* start_unwrapped = _start_val.get_vec3();
                    const math::Vec3<T>* end_unwrapped = _end_val.get_vec3();
                    assert(start_unwrapped && end_unwrapped && "Vec3 expected from unwrapping");
                    math::Vec3<T> lerp_val = math::Global::lerp(*start_unwrapped, *end_unwrapped, linear_0to1);
                    _mat.scale(lerp_val);
                    return _mat;
                } break;
                case math::TransformationType::ROTATION3d: {
                    const math::Vec3<T>* start_unwrapped = _start_val.get_vec3();
                    const math::Vec3<T>* end_unwrapped = _end_val.get_vec3();
                    assert(start_unwrapped && end_unwrapped && "Vec3 expected from unwrapping");
                    math::Vec3<T> lerp_val = math::Global::lerp(*start_unwrapped, *end_unwrapped, linear_0to1);
                    _mat.rotate3d(lerp_val);
                    return _mat;
                } break;
                case math::TransformationType::ROTATION: {
                    const T* start_unwrapped = _start_val.get_scalar();
                    const T* end_unwrapped = _end_val.get_scalar();
                    assert(start_unwrapped && end_unwrapped && "Scalar expected from unwrapping");
                    T lerp_val = math::Global::lerp(*start_unwrapped, *end_unwrapped, linear_0to1);
                    _mat.rotate(lerp_val, _axis);
                    return _mat;
                } break;
                case math::TransformationType::SHEAR: {
                    const math::VecBase<T, 2u>* start_unwrapped = _start_val.get_vec2();
                    const math::VecBase<T, 2u>* end_unwrapped = _end_val.get_vec2();
                    assert(start_unwrapped && end_unwrapped && "Vec2 expected from unwrapping");
                    math::VecBase<T, 2u> lerp_val = math::Global::lerp(*start_unwrapped, *end_unwrapped, linear_0to1);
                    _mat.shear(_axis, lerp_val);
                    return _mat;
                } break;
                default:
                    assert(false && "unreachable code reached");
                    return _mat;
            }
        }

        // should be called at the end of the current frame
        void update_time(Duration_sec frame_time) {
            if (_is_reversed) {
                frame_time *= -1.0f;
            }

            _curr_time += frame_time;

            // prevent case where delay time can be more than duration
            // to not set erroneous flags
            if (_delay.count() > 0.0f && !_is_delay_passed) {
                return;
            }

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
            else if (_curr_time < _start_time && _loop == Loop_type::INVERT) {
                _is_reversed = !_is_reversed;
            }
        }

        Bezier_curve<T>                     _bezier_curve;
        math::Matrix44<T>                   _mat;
        AnimValue<T>                        _start_val;
        AnimValue<T>                        _end_val;
        Timepoint_sec                       _start_time;
        Timepoint_sec                       _curr_time;
        Duration_sec                        _duration{1.0f};
        Duration_sec                        _delay{0.0f};
        math::Global::AXIS                  _axis;
        math::TransformationType            _anim_type;
        Loop_type                           _loop{ Loop_type::NONE };
        bool                                _is_started{ false };
        bool                                _is_delay_passed{ false };
        bool                                _is_ended{ false };
        bool                                _is_reversed{ false };
        // make sence only with pause / play system
        // bool                                _is_paused{ false };
    };
}
