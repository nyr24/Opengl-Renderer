#pragma once
#include <array>
#include <concepts>
#include <cassert>
#include <math.h>
#include "matrix.hpp"

namespace my_gl {
    enum ANIMATION_TYPE {
        TRANSLATE,
        ROTATE,
        ROTATE3d,
        SCALE
    };

    template<typename VAL_TYPE> requires std::floating_point<VAL_TYPE>
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
        Animation(
            ANIMATION_TYPE anim_type,
            float duration,
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
            , _duration{ duration }
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

        // based on curr time and duration
        my_gl_math::Matrix44<VAL_TYPE>& update(float curr_time) {
            if (!_loop && curr_time > _duration) {
                return _mat;
            }
            
            float val_for_lerp{ (fmod(curr_time, _duration)) / _duration };
            _curr_val = my_gl_math::Global::lerp<my_gl_math::Vec3<VAL_TYPE>, VAL_TYPE>(_start_val, _end_val, val_for_lerp);
            update_matrix();
            return _mat;
        }
        

    private:
        my_gl_math::Matrix44<VAL_TYPE>      _mat;
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


        // TODO: duration, bezier-curve, interpolation instead of delta (maybe)
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