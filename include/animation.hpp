#pragma once
#include "mat.hpp"
#include "vec.hpp"
#include <array>
#include <concepts>

namespace my_gl {
    enum ANIMATION_KIND {
        TRANSLATE,
        //ROTATE,
        ROTATE3d,
        SCALE
    };

    template<typename T, typename U>
    concept vec_or_num_c = std::floating_point<U> && (std::same_as<T, my_gl_math::Vec3<U>> || std::same_as<T, U>);

    template<typename FIELD_TYPE, typename VAL_TYPE, ANIMATION_KIND ANIM_TYPE> requires vec_or_num_c<FIELD_TYPE, VAL_TYPE>
    class Animation {
    public:
        Animation(
            FIELD_TYPE&& start_val,
            FIELD_TYPE&& end_val, 
            FIELD_TYPE&& delta_val,
            bool loop,
            my_gl_math::Global::AXIS axis = my_gl_math::Global::X
        ) 
            : _start_val{ std::move(start_val) }
            , _end_val{ std::move(end_val) }
            , _delta_val{ delta_val }
            , _curr_val{ _start_val }
            , _axis{ axis }
            , _loop{ loop }
        {
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
            /* case ANIMATION_KIND::ROTATE:
                _mat = my_gl_math::Matrix44<VAL_TYPE>::rotation(_curr_val, _axis);
                break;*/
            } 
        }

        const my_gl_math::Matrix44<VAL_TYPE>& update() {
            if (_is_ended) {
                return _mat;
            }
            
            for (int i = 0; i < 3; ++i) {
                if (_delta_val[i] > 0) {
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
                    _delta_val *= -1.0f;
                    std::swap(_start_val, _end_val);

                    //_mat.translate(_curr_val);
                    update_matrix();
                    return _mat;
                }
                else {
                    _is_ended = true;
                    return _mat;
                }
            }
            else {
                //_mat.translate(_curr_val);
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
        my_gl_math::Global::AXIS        _axis;
        bool                            _loop;
        bool                            _is_ended;
        
        // TODO: duration, bezier-curve

        void update_matrix() {
            switch (ANIM_TYPE) {
            case my_gl::TRANSLATE:
                _mat.translate(_curr_val);
                break;
            /* case my_gl::ROTATE:
                _mat.rotate(_curr_val, _axis);
                break; */
            case my_gl::ROTATE3d:
                _mat.rotate3d(_curr_val);
                break;
            case my_gl::SCALE:
                _mat.scale(_curr_val);
                break;
            }
        }
    };

    using Translation = my_gl::Animation<my_gl_math::Vec3<float>, float, my_gl::TRANSLATE>;
    using Rotation3d = my_gl::Animation<my_gl_math::Vec3<float>, float, my_gl::ROTATE3d>;
    //using Rotation = my_gl::Animation<float, float, my_gl::ROTATE>;
    using Scaling = my_gl::Animation<my_gl_math::Vec3<float>, float, my_gl::SCALE>;
}