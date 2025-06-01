#pragma once
#include <cmath>
#include <initializer_list>
#include <concepts>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <array>
#include <sys/types.h>
#include "math.hpp"

namespace my_gl {
namespace math {

template<std::floating_point T, uint32_t N>
class VecBase {
public:
    std::array<T, N> _data{};
public:
    constexpr VecBase<T, N>() = default;
    constexpr explicit VecBase<T, N>(T val)
    {
        _data.fill(val);
    }
    VecBase<T, N>(std::initializer_list<T> init)
    {
        assert(init.size() == N && "invalid initializer list size for this type");
        std::copy(init.begin(), init.end(), _data.begin());
    }

    // copy
    template<uint32_t N_RHS>
    VecBase<T, N>(const VecBase<T, N_RHS>& rhs)
    {
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
        constexpr uint32_t min_len = std::min(N, N_RHS);
        for (uint32_t i = 0; i < min_len; ++i) {
            _data[i] = rhs._data[i];
        }
        if constexpr (N_RHS < N) {
            assign_default_to_rest<N_RHS>();
        }
        return *this;
    }

    // move
    template<uint32_t N_RHS>
    VecBase<T, N>(VecBase<T, N_RHS>&& rhs) noexcept
    {
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
    constexpr void assign_default_to_rest() {
        for (uint32_t i = N_RHS; i < N; ++i) {
            _data[i] = T(1.0);
        }
    }

    T length() const {
        T len_squared{};
        for (auto i : _data) {
            len_squared += i * i;
        }

        return std::sqrt(len_squared);
    }

    VecBase<T, N>& normalize_inplace() {
        T vLength{ length() };
        if (vLength > 0) {
            T invLength{ 1 / vLength };
            for (uint16_t i{0}; i < N; ++i) {
                _data[i] *= invLength;
            }
        }
        return *this;
    }

    VecBase<T, N> normalize_new() const {
        auto res{ *this };
        res.normalize_inplace();
        return res;
    }

    constexpr T dot(const VecBase<T, N>& rhs) const {
        T dot{};
        for (uint16_t i{0}; i < N; ++i) {
            dot += _data[i] * rhs._data[i];
        }

        return dot;
    }

    constexpr VecBase<T, N>& negate_inplace() {
        for (uint16_t i{0}; i < N; ++i) {
            _data[i] *= T(-1.0);
        }
        return *this;
    }

    VecBase<T, N> negate_new() const {
        VecBase<T, N> res{ *this };
        res.negate_inplace();
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
        for (uint16_t i{0}; i < N; ++i) {
            res[i] += rhs[i];
        }
        return res;
    }

    VecBase<T, N> operator-(const VecBase<T, N>& rhs) const {
        VecBase<T, N> res{ *this };
        for (uint16_t i{0}; i < N; ++i) {
            res[i] -= rhs[i];
        }
        return res;
    }

    VecBase<T, N> operator*(const VecBase<T, N>& rhs) const {
        VecBase<T, N> res{ *this };
        for (uint16_t i{0}; i < N; ++i) {
            res[i] *= rhs[i];
        }
        return res;
    }

    VecBase<T, N> operator/(const VecBase<T, N>& rhs) const {
        VecBase<T, N> res{ *this };
        for (uint16_t i{0}; i < N; ++i) {
            res[i] /= rhs[i];
        }
        return res;
    }

    VecBase<T, N> operator%(const VecBase<T, N>& rhs) const {
        VecBase<T, N> res{ *this };
        for (uint16_t i{0}; i < N; ++i) {
            res[i] %= rhs[i];
        }
        return res;
    }

    VecBase<T, N>& operator+=(const VecBase<T, N>& rhs) {
        for (uint16_t i{0}; i < N; ++i) {
            _data[i] += rhs[i];
        }
        return *this;
    }

    VecBase<T, N>& operator-=(const VecBase<T, N>& rhs) {
        for (uint16_t i{0}; i < N; ++i) {
            _data[i] -= rhs[i];
        }
        return *this;
    }

    VecBase<T, N>& operator*=(const VecBase<T, N>& rhs) {
        for (uint16_t i{0}; i < N; ++i) {
            _data[i] *= rhs[i];
        }
        return *this;
    }

    VecBase<T, N>& operator/=(const VecBase<T, N>& rhs) {
        for (uint16_t i{0}; i < N; ++i) {
            _data[i] /= rhs[i];
        }
        return *this;
    }

    VecBase<T, N>& operator%=(const VecBase<T, N>& rhs) {
        for (uint16_t i{0}; i < N; ++i) {
            _data[i] %= rhs[i];
        }
        return *this;
    }

    bool cmp(const VecBase<T, N>& rhs) {
        return *this == rhs;
    }

// operations with primitives
    friend VecBase<T, N> operator+(const VecBase<T, N>& vec, T s) {
        VecBase<T, N> res{ vec };
        for (uint16_t i{0}; i < N; ++i) {
            res[i] += s;
        }
        return res;
    }

    friend VecBase<T, N> operator+(T s, const VecBase<T, N>& vec) {
        VecBase<T, N> res{ vec };
        for (uint16_t i{0}; i < N; ++i) {
            res[i] += s;
        }
        return res;
    }

    friend VecBase<T, N> operator-(const VecBase<T, N>& vec, T s) {
        VecBase<T, N> res{ vec };
        for (uint16_t i{0}; i < N; ++i) {
            res[i] -= s;
        }
        return res;
    }

    friend VecBase<T, N> operator*(const VecBase<T, N>& vec, T s) {
        VecBase<T, N> res{ vec };
        for (uint16_t i{0}; i < N; ++i) {
            res[i] *= s;
        }
        return res;
    }

    friend VecBase<T, N> operator*(T s, const VecBase<T, N>& vec) {
        VecBase<T, N> res{ vec };
        for (uint16_t i{0}; i < N; ++i) {
            res[i] *= s;
        }
        return res;
    }

    friend VecBase<T, N> operator/(const VecBase<T, N>& vec, T s) {
        VecBase<T, N> res{ vec };
        for (uint16_t i{0}; i < N; ++i) {
            res[i] /= s;
        }
        return res;
    }

    friend VecBase<T, N> operator%(const VecBase<T, N>& vec, T s) {
        VecBase<T, N> res{ vec };
        for (uint16_t i{0}; i < N; ++i) {
            res[i] %= s;
        }
        return res;
    }

    VecBase<T, N>& operator+=(T s) {
        for (uint16_t i{0}; i < N; ++i) {
            _data[i] += s;
        }
        return *this;
    }

    VecBase<T, N>& operator-=(T s) {
        for (uint16_t i{0}; i < N; ++i) {
            _data[i] -= s;
        }
        return *this;
    }

    VecBase<T, N>& operator*=(T s) {
        for (uint16_t i{0}; i < N; ++i) {
            _data[i] *= s;
        }
        return *this;
    }

    VecBase<T, N>& operator/=(T s) {
        for (uint16_t i{0}; i < N; ++i) {
            _data[i] /= s;
        }
        return *this;
    }

    VecBase<T, N>& operator%=(T s) {
        for (uint16_t i{0}; i < N; ++i) {
            _data[i] %= s;
        }
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

} // math
} // my_gl
