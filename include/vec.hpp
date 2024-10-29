#pragma once
#include <cmath>
#include <array>
#include <initializer_list>
#include <concepts>
#include "global.hpp"

namespace my_gl_math {
    template<typename T, uint32_t N> requires std::floating_point<T>
    class Vec {
    public:
        Vec(): _data(N) {}
        explicit Vec(T val): _data(val, N) {}
        Vec(std::initializer_list<T> init): _data{ init } {
            assert(init.size() == N && "invalid initializer list size for this type");
        }

        Vec(const Vec<T, N>& src) = default;
        Vec(Vec<T, N>&& src) = default;
        Vec<T, N>& operator=(const Vec<T, N>& src) = default;
        Vec<T, N>& operator=(Vec<T, N>&& src) = default;

        T length() const {
            std::valarray<T> squared_data = _data.apply([](T el) {
                return el * el;
            });

            return std::sqrt(squared_data.sum());
        }

        Vec<T, N>& normalize_inplace() {
            T vLength{ length() };
            if (vLength > 0) {
                T invLength{ 1 / vLength };
                _data *= invLength;
            }
            return *this;
        }

        Vec<T, N> normalize_new() const {
            auto res{ *this };
            res.normalize_inplace();
            return res;
        }

        T dot(const Vec<T, N>& rhs) const {
            std::valarray<T> res = _data * rhs._data;
            return res.sum();
        }
    
    // operators
        T& operator[](int i) {
            assert((i >= 0 && i < N) && "invalid indexing");
            return _data[i];
        }

        const T& operator[](int i) const {
            assert((i >= 0 && i < N) && "invalid indexing");
            return _data[i];
        }

        Vec<T, N> operator+(const Vec<T, N>& rhs) {
            Vec<T, N> res;
            res._data = _data + rhs._data;
            return res;
        }

        Vec<T, N> operator-(const Vec<T, N>& rhs) {
            Vec<T, N> res;
            res._data = _data - rhs._data;
            return res;
        }

        Vec<T, N> operator*(const Vec<T, N>& rhs) {
            Vec<T, N> res;
            res._data = _data * rhs._data;
            return res;
        }

        Vec<T, N> operator/(const Vec<T, N>& rhs) {
            Vec<T, N> res;
            res._data = _data / rhs._data;
            return res;
        }

        Vec<T, N> operator%(const Vec<T, N>& rhs) {
            Vec<T, N> res;
            res._data = _data % rhs._data;
            return res;
        }

        Vec<T, N>& operator+=(const Vec<T, N>& rhs) {
            _data += rhs._data;
            return *this;
        }

        Vec<T, N>& operator-=(const Vec<T, N>& rhs) {
            _data -= rhs._data;
            return *this;
        }

        Vec<T, N>& operator*=(const Vec<T, N>& rhs) {
            _data *= rhs._data;
            return *this;
        }

        Vec<T, N>& operator/=(const Vec<T, N>& rhs) {
            _data /= rhs._data;
            return *this;
        }

        Vec<T, N>& operator%=(const Vec<T, N>& rhs) {
            _data %= rhs._data;
            return *this;
        }

    // operations with primitives
        Vec<T, N> operator+(T val) {
            Vec<T, N> res;
            res._data = _data + val;
            return res;
        }

        Vec<T, N> operator-(T val) {
            Vec<T, N> res;
            res._data = _data - val;
            return res;
        }

        Vec<T, N> operator*(T val) {
            Vec<T, N> res;
            res._data = _data * val;
            return res;
        }

        Vec<T, N> operator/(T val) {
            Vec<T, N> res;
            res._data = _data / val;
            return res;
        }

        Vec<T, N> operator%(T val) {
            Vec<T, N> res;
            res._data = _data % val;
            return res;
        }

        Vec<T, N>& operator+=(T val) {
            _data += val;
            return *this;
        }

        Vec<T, N>& operator-=(T val) {
            _data -= val;
            return *this;
        }

        Vec<T, N>& operator*=(T val) {
            _data *= val;
            return *this;
        }

        Vec<T, N>& operator/=(T val) {
            _data /= val;
            return *this;
        }

        Vec<T, N>& operator%=(T val) {
            _data %= val;
            return *this;
        }

    // utility
        friend std::ostream& operator<<(std::ostream& out, const Vec<T, N>& vec) {
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

        bool cmp(const Vec<T, N>& rhs) {
            for (int i = 0; i < N; ++i) {
			    if (!my_gl_math::Global::cmp_float<T>(_data[i], rhs._data[i]))
				    return false;
            }

            return true;
        }

    protected:
        std::valarray<T> _data{};
    };


// Vec4
    template<typename T = float> requires std::floating_point<T>
    class Vec4 : public Vec<T, 4> {
    public:
        using Vec<T, 4>::Vec;

        constexpr T x() const { return this->_data[0]; }
        constexpr T y() const { return this->_data[1]; }
        constexpr T z() const { return this->_data[2]; }
        constexpr T w() const { return this->_data[3]; }

        Vec4 cross(const Vec4& rhs) const {
            return Vec4{
                y() * rhs.z() - z() * rhs.y(),
                z() * rhs.x() - x() * rhs.z(),
                x() * rhs.y() - y() * rhs.x(),
                0  // W component remains unchanged
            };     
        }
    };


// Vec3
    template<typename T = float> requires std::floating_point<T>
    class Vec3 : public Vec<T, 3> {
    public:
        using Vec<T, 3>::Vec;

        constexpr T x() const { return this->_data[0]; }
        constexpr T y() const { return this->_data[1]; }
        constexpr T z() const { return this->_data[2]; }

        Vec3 cross(const Vec3& rhs) const {
            return Vec3{
                y() * rhs.z() - z() * rhs.y(),
                z() * rhs.x() - x() * rhs.z(),
                x() * rhs.y() - y() * rhs.x(),
            };     
        }
    };
}