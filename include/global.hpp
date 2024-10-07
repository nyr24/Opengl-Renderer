#pragma once
#include <valarray>
#include <limits>
#include <concepts>

namespace my_gl_math {
    template<typename T> requires std::floating_point<T>
    class Vec3;

    class Global {
    public:
        static constexpr double PI = 3.14159;
        
        enum AXIS {
            X,
            Y,
            Z,
        };

        template<typename T>
        struct Spherical_coords {
            T length;
            T theta_deg;
            T phi_deg;
        };
    
        template<typename T>
        static constexpr T degToRad(T deg) {
            return deg * static_cast<T>(Global::DEG_TO_RAD);
        }

        template<typename T>
        static constexpr T radToDeg(T rad) {
            return rad * static_cast<T>(Global::RAD_TO_DEG);
        }

        template<typename T>
        static constexpr Vec3<T> spher_to_cart(const Spherical_coords<T>& spher_coords) {
            Vec3<T> res;
            
            T theta_rad{ degToRad(spher_coords.length) };
            T phi_rad{ degToRad(spher_coords.phi_deg) };
            
            res[0] = spher_coords.length * std::sin(theta_rad) * std::cos(phi_rad);
            res[1] = spher_coords.length * std::sin(theta_rad) * std::sin(phi_rad);
            res[2] = spher_coords.length * std::cos(phi_rad);

            return res;
        }

        template<typename T>
        static constexpr Spherical_coords<T> cart_to_spher(const Vec3<T>& cart_coords) {
            Spherical_coords spher_coords{ .length = cart_coords.length() };

            spher_coords.theta_deg = std::acos(cart_coords[2]);
            spher_coords.phi_deg = std::atan2(cart_coords[1], cart_coords[0]);

            return spher_coords;
        }

        template<typename T>
        static constexpr T spher_theta(const Vec3<T>& cart_coords) {
            return std::acos(std::clamp<T>(cart_coords[2], -1, 1));
        }

        template<typename T>
        static constexpr T spher_phi(const Vec3<T>& cart_coords) {
            T phi_res = std::atan2(cart_coords[1], cart_coords[0]);
            return (phi_res < 0) ? phi_res + 2 * PI : phi_res;
        }

        template <std::floating_point T>
        static bool cmp_float(T x, T y, int precision = 2) {
            if (std::abs(x - y) <= std::numeric_limits<T>::epsilon())
                return true;
            return std::abs(x - y) <= std::numeric_limits<T>::epsilon() * std::max(std::abs(x), std::abs(y));
        }
        /*  cmp_float 2
            template <std::floating_point T>
            bool AreEqual(T x, T y, int precision = 2)
            {
            // Scale the machine epsilon to the magnitude of the given values and
            // multiply by the required precision.
            return fabs(x - y) <= numeric_limits<T>::epsilon() * fabs(x + y) * precision
            || fabs(x - y) < numeric_limits<T>::min(); // The result is subnormal.
            }
        */

    private:
        static constexpr double DEG_TO_RAD = PI / 180;
        static constexpr double RAD_TO_DEG = 180 / PI;
    };
}