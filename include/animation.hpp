#pragma once
#include "mat.hpp"
#include "vec.hpp"
#include <array>
#include <concepts>

namespace my_gl {
    enum ANIMATION_KIND {
        TRANSLATE,
        ROTATE,
        ROTATE3d,
        SCALE
    };

    template<typename T, typename U>
    concept is_vec = std::floating_point<U> && std::same_as<T, my_gl_math::Vec3<U>>;

    template<typename FIELD_TYPE, typename VAL_TYPE, ANIMATION_KIND ANIM_TYPE> requires is_vec<FIELD_TYPE, VAL_TYPE>
    class Animation {
    public:
        Animation(
            FIELD_TYPE&& start_val,
            FIELD_TYPE&& end_val, 
            FIELD_TYPE&& delta_val,
            bool loop
        ) 
            : _start_val{ std::move(start_val) }
            , _end_val{ std::move(end_val) }
            , _delta_val{ delta_val }
            , _curr_val{ _start_val }
            , _loop{ loop }
        {
            static_assert(ANIM_TYPE != ANIMATION_KIND::ROTATE);

            switch (ANIM_TYPE) {
            case ANIMATION_KIND::TRANSLATE:
                _mat = my_gl_math::Matrix44<VAL_TYPE>::translation(_curr_val);
                break;
            case ANIMATION_KIND::SCALE:
                _mat = my_gl_math::Matrix44<VAL_TYPE>::scaling(_curr_val);
                break;
            case ANIMATION_KIND::ROTATE3d:
                _mat = my_gl_math::Matrix44<VAL_TYPE>::rotation3d(_curr_val);
                break;
            }
        }

    // constructor for scalar (float/int/double) input
    // useful in rotation around single axis
        Animation(
            VAL_TYPE start_val,
            VAL_TYPE end_val, 
            VAL_TYPE delta_val,
            my_gl_math::Global::AXIS axis,
            bool loop
        ) 
            : _axis{ axis }
            , _loop{ loop }
        {
            static_assert(ANIM_TYPE == ANIMATION_KIND::ROTATE);

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


        const my_gl_math::Matrix44<VAL_TYPE>& update() {
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
    private:
        my_gl_math::Matrix44<VAL_TYPE>  _mat;
        FIELD_TYPE                      _start_val;
        FIELD_TYPE                      _end_val;
        FIELD_TYPE                      _delta_val;
        FIELD_TYPE                      _curr_val;
        // only for rotation
        my_gl_math::Global::AXIS        _axis{ my_gl_math::Global::Z };
        bool                            _loop{ false };
        bool                            _is_ended{ false };
        // TODO: duration, bezier-curve

        void update_matrix() {
            switch (ANIM_TYPE) {
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

    using Translation = my_gl::Animation<my_gl_math::Vec3<float>, float, my_gl::TRANSLATE>;
    using Rotation3d = my_gl::Animation<my_gl_math::Vec3<float>, float, my_gl::ROTATE3d>;
    using Rotation = my_gl::Animation<my_gl_math::Vec3<float>, float, my_gl::ROTATE>;
    using Scaling = my_gl::Animation<my_gl_math::Vec3<float>, float, my_gl::SCALE>;
}