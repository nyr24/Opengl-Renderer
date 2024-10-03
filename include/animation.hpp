#pragma once
#include "mat.hpp"
#include "vec.hpp"
#include <array>
#include <optional>

namespace my_gl {
    class IAnimationBase {
    public:
        virtual ~IAnimationBase() = default;
        virtual const my_gl_math::Matrix44<float>* const update() = 0;
        //virtual const float* data() const = 0;
        //virtual const my_gl_math::Matrix44<float>* const mat() const = 0;
    protected:
        //virtual bool check_borders() const = 0;
        my_gl_math::Matrix44<float> _mat;
        float                       _start_val;
        float                       _end_val;
        float                       _delta;
        float                       _curr_val;
        bool                        _loop;
        bool                        _is_ended;
        //bool                        _is_started;
    };

    /* class Rotation : public IAnimationBase {
    public:
        Rotation(
            float start_angle_deg, 
            float end_angle_deg, 
            float delta_deg, 
            my_gl_math::Global::AXIS axis
        );
        void update() override;
        const float* data() const override;
        const my_gl_math::Matrix44<float>& mat() const override;
    private:
        bool check_borders() const override;
        my_gl_math::Matrix44<float> _mat;
        float                    _start_angle;
        float                    _end_angle;
        float                    _delta;
        float                    _curr_angle;
        my_gl_math::Global::AXIS _axis;
    }; */

    class Translation : public IAnimationBase {
    public:
        Translation(
            const my_gl_math::Vec3<float>&& from_vec,
            const my_gl_math::Vec3<float>&& to_vec, 
            const my_gl_math::Vec3<float>&& delta,
            bool loop
        );
        const my_gl_math::Matrix44<float>* const update() override;
        //const float* data() const override;
        //const my_gl_math::Matrix44<float>& mat() const override;
    private:
        //bool check_borders() const override;
        my_gl_math::Matrix44<float> _mat;
        my_gl_math::Vec3<float>     _from_vec;
        my_gl_math::Vec3<float>     _to_vec;
        my_gl_math::Vec3<float>     _delta;
        my_gl_math::Vec3<float>     _curr_val;
        bool                        _loop;
        bool                        _is_ended{ false };
    };
}