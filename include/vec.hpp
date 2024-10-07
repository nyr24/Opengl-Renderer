#pragma once
#include <cmath>
#include <array>
#include <initializer_list>
#include <concepts>
#include "global.hpp"

namespace my_gl_math {
    template<typename T, typename DERIVED_T, int EL_COUNT> requires std::floating_point<T>
    class Vec {
    public:
        Vec(): _data(EL_COUNT) {}
        explicit Vec(T val): _data(val, EL_COUNT) {}
        Vec(std::initializer_list<T> init): _data{ init } {
            assert(init.size() == EL_COUNT && "invalid initializer list size for this type");
        }

        Vec(const Vec<T, DERIVED_T, EL_COUNT>& src) = default;
        Vec(Vec<T, DERIVED_T, EL_COUNT>&& src) = default;
        Vec<T, DERIVED_T, EL_COUNT>& operator=(const Vec<T, DERIVED_T, EL_COUNT>& src) = default;
        Vec<T, DERIVED_T, EL_COUNT>& operator=(Vec<T, DERIVED_T, EL_COUNT>&& src) = default;

        T length() const {
            std::valarray<T> squared_data = _data.apply([](T el) {
                return el * el;
            });

            return std::sqrt(squared_data.sum());
        }

        Vec<T, DERIVED_T, EL_COUNT>& normalize() {
            T vLength{ length() };
            if (vLength > 0) {
                T invLength{ 1 / vLength };
                _data *=  invLength;
            }
            return *this;
        }

        T dot(const Vec<T, DERIVED_T, EL_COUNT>& rhs) const {
            std::valarray<T> res = _data * rhs._data;
            return res.sum();
        }
    
    // operators
        T& operator[](int i) {
            assert((i >= 0 && i < size()) && "invalid indexing");
            return _data[i];
        }

        const T& operator[](int i) const {
            assert((i >= 0 && i < size()) && "invalid indexing");
            return _data[i];
        }

        DERIVED_T operator+(const DERIVED_T& rhs) {
            DERIVED_T res;
            res._data = _data + rhs._data;
            return res;
        }

        DERIVED_T operator-(const DERIVED_T& rhs) {
            DERIVED_T res;
            res._data = _data - rhs._data;
            return res;
        }

        DERIVED_T operator*(const DERIVED_T& rhs) {
            DERIVED_T res;
            res._data = _data * rhs._data;
            return res;
        }

        DERIVED_T operator/(const DERIVED_T& rhs) {
            DERIVED_T res;
            res._data = _data / rhs._data;
            return res;
        }

        DERIVED_T operator%(const DERIVED_T& rhs) {
            DERIVED_T res;
            res._data = _data % rhs._data;
            return res;
        }

        DERIVED_T& operator+=(const DERIVED_T& rhs) {
            _data += rhs._data;
            return static_cast<DERIVED_T&>(*this);
        }

        DERIVED_T& operator-=(const DERIVED_T& rhs) {
            _data -= rhs._data;
            return static_cast<DERIVED_T&>(*this);
        }

        DERIVED_T& operator*=(const DERIVED_T& rhs) {
            _data *= rhs._data;
            return static_cast<DERIVED_T&>(*this);
        }

        DERIVED_T& operator/=(const DERIVED_T& rhs) {
            _data /= rhs._data;
            return static_cast<DERIVED_T&>(*this);
        }

        DERIVED_T& operator%=(const DERIVED_T& rhs) {
            _data %= rhs._data;
            return static_cast<DERIVED_T&>(*this);
        }

    // operations with primitives
        DERIVED_T operator+(T val) {
            DERIVED_T res;
            res._data = _data + val;
            return res;
        }

        DERIVED_T operator-(T val) {
            DERIVED_T res;
            res._data = _data - val;
            return res;
        }

        DERIVED_T operator*(T val) {
            DERIVED_T res;
            res._data = _data * val;
            return res;
        }

        DERIVED_T operator/(T val) {
            DERIVED_T res;
            res._data = _data / val;
            return res;
        }

        DERIVED_T operator%(T val) {
            DERIVED_T res;
            res._data = _data % val;
            return res;
        }

        DERIVED_T& operator+=(T val) {
            _data += val;
            return static_cast<DERIVED_T&>(*this);
        }

        DERIVED_T& operator-=(T val) {
            _data -= val;
            return static_cast<DERIVED_T&>(*this);
        }

        DERIVED_T& operator*=(T val) {
            _data *= val;
            return static_cast<DERIVED_T&>(*this);
        }

        DERIVED_T& operator/=(T val) {
            _data /= val;
            return static_cast<DERIVED_T&>(*this);
        }

        DERIVED_T& operator%=(T val) {
            _data %= val;
            return static_cast<DERIVED_T&>(*this);
        }

        friend std::ostream& operator<<(std::ostream& out, const DERIVED_T& vec) {
            for (const T el : vec._data) {
                out << el << ' ';
            }
            out << '\n';
            return out;
        }

    // utility
        constexpr T size() const { return EL_COUNT; }

        void print() const {
            for (const T el : _data) {
                std::cout << el << ' ';
            } 
            std::cout << '\n';
        }

        bool cmp(const Vec<T, DERIVED_T, EL_COUNT>& rhs) {
            for (int i = 0; i < EL_COUNT; ++i) {
			    if (!my_gl_math::Global::cmp_float<T>(_data[i], rhs._data[i]))
				    return false;
            }

            return true;
        }

    protected:
        std::valarray<T> _data;
    };


// Vec4
    template<typename T = float> requires std::floating_point<T>
    class Vec4 : public Vec<T, Vec4<T>, 4> {
    public:
        using Vec<T, Vec4<T>, 4>::Vec;
        using Vec<T, Vec4<T>, 4>::operator=;

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
    class Vec3 : public Vec<T, Vec3<T>, 3> {
    public:
        using Vec<T, Vec3<T>, 3>::Vec;
        using Vec<T, Vec3<T>, 3>::operator=;

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