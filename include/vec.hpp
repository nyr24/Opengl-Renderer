#pragma once
#include <cmath>
#include <array>
#include <initializer_list>
#include <concepts>
#include <cstdint>
#include "math.hpp"

namespace my_gl_math {
    template<typename T, uint32_t N> requires std::floating_point<T>
    class VecBase {
    public:
        VecBase(): _data(N) {}
        explicit VecBase(T val): _data(val, N) {}
        VecBase(std::initializer_list<T> init): _data{ init } {
            assert(init.size() == N && "invalid initializer list size for this type");
        }
        VecBase(T* values): VecBase() {
            assert(values + (N - 1) && "not enough values to initialize a vector");
            memcpy(&_data[0], values, sizeof(T) * N);
        }

        // copy
        VecBase(const VecBase<T, N>& src) = default;
        VecBase<T, N>& operator=(const VecBase<T, N>& src) = default;
        // move
        VecBase(VecBase<T, N>&& src) = default;
        VecBase<T, N>& operator=(VecBase<T, N>&& src) = default;

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

        T dot(const VecBase<T, N>& rhs) const {
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

        VecBase<T, N> operator+(const VecBase<T, N>& rhs) const {
            VecBase<T, N> res{ *this };
            res._data = _data + rhs._data;
            return res;
        }

        VecBase<T, N> operator-(const VecBase<T, N>& rhs) const {
            VecBase<T, N> res{ *this };
            res._data = _data - rhs._data;
            return res;
        }

        VecBase<T, N> operator*(const VecBase<T, N>& rhs) const {
            VecBase<T, N> res{ *this };
            res._data = _data * rhs._data;
            return res;
        }

        VecBase<T, N> operator/(const VecBase<T, N>& rhs) const {
            VecBase<T, N> res{ *this };
            res._data = _data / rhs._data;
            return res;
        }

        VecBase<T, N> operator%(const VecBase<T, N>& rhs) const {
            VecBase<T, N> res{ *this };
            res._data = _data % rhs._data;
            return res;
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

    // operations with primitives
        VecBase<T, N> operator+(T val) const {
            VecBase<T, N> res{ *this };;
            res._data = _data + val;
            return res;
        }

        VecBase<T, N> operator-(T val) const {
            VecBase<T, N> res{ *this };
            res._data = _data - val;
            return res;
        }

        VecBase<T, N> operator*(T val) const {
            VecBase<T, N> res{ *this };
            res._data = _data * val;
            return res;
        }

        VecBase<T, N> operator/(T val) const {
            VecBase<T, N> res{ *this };
            res._data = _data / val;
            return res;
        }

        VecBase<T, N> operator%(T val) const {
            VecBase<T, N> res{ *this };
            res._data = _data % val;
            return res;
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

        bool cmp(const VecBase<T, N>& rhs) {
            for (int i = 0; i < N; ++i) {
			    if (!my_gl_math::Global::cmp_float<T>(_data[i], rhs._data[i]))
				    return false;
            }

            return true;
        }

        // public because can't access it any other way from derived classes while having it inside of the 'protected' label (wtf)
        std::valarray<T> _data{};
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

        Vec3 cross(const Vec3& rhs) const {
            return Vec3{
                y() * rhs.z() - z() * rhs.y(),
                z() * rhs.x() - x() * rhs.z(),
                x() * rhs.y() - y() * rhs.x(),
            };     
        }
    };
}