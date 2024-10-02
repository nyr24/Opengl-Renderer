#pragma once

namespace my_gl_math {
    class Global {
    public:
        static constexpr double PI = 3.14159;
    
        template<typename T>
        static constexpr T degToRad(T deg) {
            return deg * static_cast<T>(Global::DEG_TO_RAD);
        }

        template<typename T>
        constexpr T radToDeg(T rad) {
            return rad * static_cast<T>(Global::RAD_TO_DEG);
        }

        enum AXIS {
            x,
            y,
            z
        };

    private:
        static constexpr double DEG_TO_RAD = PI / 180;
        static constexpr double RAD_TO_DEG = 180 / PI;
    };
}