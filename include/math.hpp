#pragma once
#include "sharedTypes.hpp"
#include <limits>
#include <concepts>
#include <math.h>

namespace my_gl {
    namespace math {
        template<typename T> requires std::floating_point<T>
        class Vec3;

        template<typename T> requires std::floating_point<T>
        class Vec4;

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

                T theta_rad{ degToRad(spher_coords.theta_deg) };
                T phi_rad{ degToRad(spher_coords.phi_deg) };

                res[0] = spher_coords.length * std::sin(theta_rad) * std::cos(phi_rad);
                res[1] = spher_coords.length * std::sin(theta_rad) * std::sin(phi_rad);
                res[2] = spher_coords.length * std::cos(phi_rad);

                return res;
            }

            template<typename T>
            static constexpr Spherical_coords<T> cart_to_spher(Vec3<T>& cart_coords) {
                Spherical_coords spher_coords{ .length = cart_coords.length() };

                auto normalized_vec{ cart_coords.normalize_new() };

                spher_coords.theta_deg = std::acos(normalized_vec[2]);
                spher_coords.phi_deg = std::atan2(normalized_vec[1], normalized_vec[0]);

                return spher_coords;
            }

            template<typename T>
            static constexpr T spher_theta(Vec3<T>& cart_coords) {
                return std::acos(std::clamp<T>(cart_coords[2], -1, 1));
            }

            template<typename T>
            static constexpr T spher_phi(Vec3<T>& cart_coords) {
                T phi_res = std::atan2(cart_coords[1], cart_coords[0]);
                return (phi_res < 0) ? phi_res + 2 * PI : phi_res;
            }

            template <std::floating_point T>
            static bool cmp_float(T x, T y) {
                if (std::abs(x - y) <= std::numeric_limits<T>::epsilon())
                    return true;
                return std::abs(x - y) <= std::numeric_limits<T>::epsilon() * std::max(std::abs(x), std::abs(y));
            }

            template<typename T, std::floating_point N>
            static T lerp(T start, T end, N coefficient) {
                return (start * (1 - coefficient)) + (end * coefficient);
            }

            template<std::integral T>
            static T factorial(T n) {
                T res{ 1 };

                for (T i = n; i > 0; --i) {
                    res *= i;
                }

                return res;
            }

            template<std::integral T>
            static T binomial_coef(T i, T n) {
                T denom{ factorial(i) * factorial(n - i) };
                return factorial(n) / denom;
            }

            template<std::integral T, std::floating_point N>
            static float bernstein_polynomial(T i, T n, N lerp) {
                T binom_coef{ binomial_coef(i, n) };
                return binom_coef * std::pow<N, T>(lerp, i) * std::pow<N, T>((static_cast<N>(1) - lerp), n - i);
            }

            template<std::floating_point T>
            static Vec3<T> monomial_basis_quad(T t) {
                return Vec3<T>{ (t * t), t, static_cast<T>(1.0) };
            }

            template<std::floating_point T>
            static Vec4<T> monomial_basis_cube(T t) {
                return Vec4<T>{ (t * t * t), (t * t), t, static_cast<T>(1.0) };
            }

            static float map_duration_to01(my_gl::Duration_sec input) {
                return std::sin(input.count()) * 0.5f + 0.5f;
            }

            template<std::floating_point T, typename Range>
            static T clamp_val_to_range_loop(T input, Range start, Range end) {
                Range out_distance_ratio{ (end - start) / Range(2) };
                T val_0_to_2{ std::sin(input) + T(1) };

                return val_0_to_2 * out_distance_ratio + start;
            }

            private:
                static constexpr double DEG_TO_RAD = PI / 180;
                static constexpr double RAD_TO_DEG = 180 / PI;
        };
    }
}
