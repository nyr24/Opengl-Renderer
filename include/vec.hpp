#pragma once
#include <cmath>
#include <initializer_list>
#include <concepts>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <valarray>
#include "math.hpp"

namespace my_gl {
    namespace math {
        template<std::floating_point T, uint32_t N>
        class VecBase {
        public:
            constexpr VecBase<T, N>(): _data(N) {}
            constexpr explicit VecBase<T, N>(T val): _data(val, N) {}
            VecBase<T, N>(std::initializer_list<T> init): _data{ init } {
                assert(init.size() == N && "invalid initializer list size for this type");
            }

            // copy
            template<uint32_t N_RHS>
            VecBase<T, N>(const VecBase<T, N_RHS>& rhs): _data(N) {
                constexpr uint32_t min_len = std::min(N, N_RHS);
                for (uint32_t i = 0; i < min_len; ++i) {
                    _data[i] = rhs._data[i];
                }
                if constexpr (N_RHS < N) {
                    assign_default_to_rest<N_RHS>();
                }
            }

            template<uint32_t N_RHS>
            VecBase<T, N>& operator=(const VecBase<T, N_RHS>& rhs)
            {
                for (uint32_t i = 0; i < N_RHS; ++i) {
                    _data[i] = rhs._data[i];
                }
                if constexpr (N_RHS < N) {
                    assign_default_to_rest<N_RHS>();
                }
                return *this;
            }

            // move
            template<uint32_t N_RHS>
            VecBase<T, N>(VecBase<T, N_RHS>&& rhs) noexcept : _data(N) {
                constexpr uint32_t min_len = std::min(N, N_RHS);
                for (uint32_t i = 0; i < min_len; ++i) {
                    _data[i] = rhs._data[i];
                }
                if constexpr (N_RHS < N) {
                    assign_default_to_rest<N_RHS>();
                }
            }

            template<uint32_t N_RHS>
            VecBase<T, N>& operator=(VecBase<T, N_RHS>&& rhs) noexcept
            {
                for (uint32_t i = 0; i < N_RHS; ++i) {
                    _data[i] = rhs._data[i];
                }
                if constexpr (N_RHS < N) {
                    assign_default_to_rest<N_RHS>();
                }
                return *this;
            }

            template<uint32_t N_RHS>
            void assign_default_to_rest() {
                for (uint32_t i = N_RHS; i < N; ++i) {
                    _data[i] = T(1.0);
                }
            }

            T length() const {
                std::valarray<T> squared_data = _data.apply([](T el) {
                    return el * el;
                });

                return std::sqrt(squared_data.sum());
            }

            VecBase<T, N>& normalize_inplace() {
                T vLength{ length() };
                if (vLength > 0) {
                    T invLength{ 1 / vLength };
                    _data *= invLength;
                }
                return *this;
            }

            VecBase<T, N> normalize_new() const {
                auto res{ *this };
                res.normalize_inplace();
                return res;
            }

            constexpr T dot(const VecBase<T, N>& rhs) const {
                std::valarray<T> res = _data * rhs._data;
                return res.sum();
            }

            constexpr VecBase<T, N>& negate_inplace() {
                _data *= static_cast<T>(-1.0);
                return *this;
            }

            VecBase<T, N> negate_new() const {
                VecBase<T, N> res{ *this };
                res._data *= static_cast<T>(-1.0);
                return res;
            }

            T& operator[](uint32_t i) {
                assert((i >= 0 && i < N) && "invalid indexing");
                return _data[i];
            }

            const T& operator[](uint32_t i) const {
                assert((i >= 0 && i < N) && "invalid indexing");
                return _data[i];
            }

            VecBase<T, N> operator+(const VecBase<T, N>& rhs) const {
                VecBase<T, N> res{ *this };
                res._data += rhs._data;
                return *this;
            }

            VecBase<T, N> operator-(const VecBase<T, N>& rhs) const {
                VecBase<T, N> res{ *this };
                res._data -= rhs._data;
                return *this;
            }

            VecBase<T, N> operator*(const VecBase<T, N>& rhs) const {
                VecBase<T, N> res{ *this };
                res._data *= rhs._data;
                return *this;
            }

            VecBase<T, N> operator/(const VecBase<T, N>& rhs) const {
                VecBase<T, N> res{ *this };
                res._data /= rhs._data;
                return *this;
            }

            VecBase<T, N> operator%(const VecBase<T, N>& rhs) const {
                VecBase<T, N> res{ *this };
                res._data %= rhs._data;
                return *this;
            }

            VecBase<T, N>& operator+=(const VecBase<T, N>& rhs) {
                _data += rhs._data;
                return *this;
            }

            VecBase<T, N>& operator-=(const VecBase<T, N>& rhs) {
                _data -= rhs._data;
                return *this;
            }

            VecBase<T, N>& operator*=(const VecBase<T, N>& rhs) {
                _data *= rhs._data;
                return *this;
            }

            VecBase<T, N>& operator/=(const VecBase<T, N>& rhs) {
                _data /= rhs._data;
                return *this;
            }

            VecBase<T, N>& operator%=(const VecBase<T, N>& rhs) {
                _data %= rhs._data;
                return *this;
            }

            bool cmp(const VecBase<T, N>& rhs) {
                return (*this == rhs);
            }

        // operations with primitives
            friend VecBase<T, N>& operator+(VecBase<T, N>& vec, T s) {
                vec._data += s;
                return vec;
            }

            friend VecBase<T, N>& operator+(T s, VecBase<T, N>& vec) {
                vec._data += s;
                return vec;
            }

            friend VecBase<T, N> operator+(const VecBase<T, N>& vec, T s) {
                VecBase<T, N> res{ vec };
                res._data += s;
                return res;
            }

            friend VecBase<T, N> operator+(T s, const VecBase<T, N>& vec) {
                VecBase<T, N> res{ vec };
                res._data += s;
                return res;
            }

            friend VecBase<T, N> operator+(VecBase<T, N>&& vec, T s) {
                vec._data += s;
                return std::move(vec);
            }
 
            friend VecBase<T, N> operator+(T s, VecBase<T, N>&& vec) {
                vec._data += s;
                return std::move(vec);
            }

            friend VecBase<T, N>& operator-(VecBase<T, N>& vec, T s) {
                vec._data -= s;
                return vec;
            }

            friend VecBase<T, N> operator-(const VecBase<T, N>& vec, T s) {
                VecBase<T, N> res{ vec };
                res._data -= s;
                return res;
            }

            friend VecBase<T, N> operator-(VecBase<T, N>&& vec, T s) {
                vec._data -= s;
                return std::move(vec);
            }

            friend VecBase<T, N>& operator*(VecBase<T, N>& vec, T s) {
                vec._data *= s;
                return vec;
            }

            friend VecBase<T, N>& operator*(T s, VecBase<T, N>& vec) {
                vec._data *= s;
                return vec;
            }

            friend VecBase<T, N> operator*(const VecBase<T, N>& vec, T s) {
                VecBase<T, N> res{ vec };
                res._data *= s;
                return res;
            }

            friend VecBase<T, N> operator*(T s, const VecBase<T, N>& vec) {
                VecBase<T, N> res{ vec };
                res._data *= s;
                return res;
            }

            friend VecBase<T, N> operator*(VecBase<T, N>&& vec, T s) {
                vec._data *= s;
                return std::move(vec);
            }
 
            friend VecBase<T, N> operator*(T s, VecBase<T, N>&& vec) {
                vec._data *= s;
                return std::move(vec);
            }

            friend VecBase<T, N>& operator/(VecBase<T, N>& vec, T s) {
                vec._data /= s;
                return vec;
            }

            friend VecBase<T, N> operator/(const VecBase<T, N>& vec, T s) {
                VecBase<T, N> res{ vec };
                res._data /= s;
                return res;
            }

            friend VecBase<T, N> operator/(VecBase<T, N>&& vec, T s) {
                vec._data /= s;
                return std::move(vec);
            }

            friend VecBase<T, N>& operator%(VecBase<T, N>& vec, T s) {
                vec._data %= s;
                return vec;
            }

            friend VecBase<T, N> operator%(const VecBase<T, N>& vec, T s) {
                VecBase<T, N> res{ vec };
                res._data %= s;
                return res;
            }

            friend VecBase<T, N> operator%(VecBase<T, N>&& vec, T s) {
                vec._data %= s;
                return std::move(vec);
            }

            VecBase<T, N>& operator+=(T val) {
                _data += val;
                return *this;
            }

            VecBase<T, N>& operator-=(T val) {
                _data -= val;
                return *this;
            }

            VecBase<T, N>& operator*=(T val) {
                _data *= val;
                return *this;
            }

            VecBase<T, N>& operator/=(T val) {
                _data /= val;
                return *this;
            }

            VecBase<T, N>& operator%=(T val) {
                _data %= val;
                return *this;
            }

        // utility
            friend std::ostream& operator<<(std::ostream& out, const VecBase<T, N>& vec) {
                for (const T el : vec._data) {
                    out << el << ' ';
                }
                out << '\n';
                return out;
            }

            constexpr int size() const { return N; }

            void print() const {
                for (const T el : _data) {
                    std::cout << el << ' ';
                } 
                std::cout << '\n';
            }

            bool cmp(const VecBase<T, N>& rhs) const {
                for (int i = 0; i < N; ++i) {
                    if (!my_gl::math::Global::cmp_float<T>(_data[i], rhs._data[i]))
                        return false;
                }

                return true;
            }

            // public because can't access it any other way from derived classes while having it inside of the 'protected' label (wtf)
            std::valarray<T> _data{};
        };

        // Vec3
        template<typename T = float> requires std::floating_point<T>
        class Vec3 : public VecBase<T, 3> {
        public:
            // ctors
            using VecBase<T, 3>::VecBase;
            Vec3(const VecBase<T, 3>& base_ref)
                : VecBase<T, 3>{ base_ref }
            {}
            Vec3(VecBase<T, 3>&& base_ref)
                : VecBase<T, 3>{ std::move(base_ref) }
            {}

            // assignment
            Vec3<T>& operator=(const VecBase<T, 3>& base_ref) {
                this->_data = base_ref._data;
                return *this;
            }
            Vec3<T>& operator=(VecBase<T, 3>&& base_ref) {
                this->_data = std::move(base_ref._data);
                return *this;
            }

            constexpr T x() const { return this->_data[0]; }
            constexpr T y() const { return this->_data[1]; }
            constexpr T z() const { return this->_data[2]; }

            Vec3<T> cross(const Vec3<T>& rhs) const {
                return Vec3<T>{
                    y() * rhs.z() - z() * rhs.y(),
                    z() * rhs.x() - x() * rhs.z(),
                    x() * rhs.y() - y() * rhs.x(),
                };
            }
        };

    // Vec4
        template<typename T = float> requires std::floating_point<T>
        class Vec4 : public VecBase<T, 4> {
        public:
            // ctors
            using VecBase<T, 4>::VecBase;
            Vec4(const VecBase<T, 4>& base_ref)
                : VecBase<T, 4>{ base_ref }
            {}
            Vec4(VecBase<T, 4>&& base_ref)
                : VecBase<T, 4>{ std::move(base_ref) }
            {}

            // assignment
            Vec4<T>& operator=(const VecBase<T, 4>& base_ref) {
                this->_data = base_ref._data;
                return *this;
            }
            Vec4<T>& operator=(VecBase<T, 4>&& base_ref) {
                this->_data = std::move(base_ref._data);
                return *this;
            }

            constexpr T x() const { return this->_data[0]; }
            constexpr T y() const { return this->_data[1]; }
            constexpr T z() const { return this->_data[2]; }
            constexpr T w() const { return this->_data[3]; }

            Vec4<T> cross(const Vec4<T>& rhs) const {
                return Vec4<T>{
                    y() * rhs.z() - z() * rhs.y(),
                    z() * rhs.x() - x() * rhs.z(),
                    x() * rhs.y() - y() * rhs.x(),
                    0  // W component remains unchanged
                };
            }
        };

    }
}
