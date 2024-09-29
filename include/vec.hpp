#pragma once
#include <cmath>
#include <array>
#include <initializer_list>

namespace gl_math {
    template<typename T, int size>
    class Vec {
    public:
        Vec(): _data{ 0 } {}; 
        Vec(T xx): _data{ xx } {};
        Vec(std::initializer_list<T> list) {
            std::copy(list.begin(), list.end(), _data.begin());
        }

        Vec(const Vec<T, size>& src) = default;
        Vec(Vec<T, size>&& src) = default;
        Vec<T, size>& operator=(const Vec<T, size>& src) = default;
        Vec<T, size>& operator=(Vec<T, size>&& src) = default;


        T length() const {
            int res = 0;
            for (const auto el : _data) {
                res += el * el;
            }
            return sqrt(res);
        }

        Vec<T, size>& normalize() {
            T vLength{ length() };
            if (vLength > 0) {
                T invLength{ 1 / vLength };
                for (const auto& el : _data) {
                    el *= invLength;
                }
            }
            return *this;
        }

        T dot(const Vec<T, size>& rhs) const {
            int dotRes = 0;
            for (int i = 0; i < size; ++i) {
                dotRes += _data[i] * rhs._data[i];
            }

            return dotRes;
        }

    // utility
    public:
        T getSize() const { return _data.size(); }

        void print() const {
            for (const T el : _data) {
                std::cout << el << ' ';
            } 
            std::cout << '\n';
        }

    protected:
        std::array<T, size> _data;
    };


// Vec4
    template<typename T = float>
    class Vec4 : public Vec<T, 4> {
    public:
        Vec4()
            : Vec<T, 4>{}
        {}
        Vec4(std::initializer_list<T> list)
            : Vec<T, 4>{ list }
        {}

        constexpr T x() const { return this->_data[0]; }
        constexpr T y() const { return this->_data[1]; }
        constexpr T z() const { return this->_data[2]; }
        constexpr T w() const { return this->_data[3]; }
        constexpr int size() const { return 4;} 

        Vec4 cross(const Vec4& rhs) const {
            return Vec4{
                y() * rhs.z() - z() * rhs.y(),
                z() * rhs.x() - x() * rhs.z(),
                x() * rhs.y() - y() * rhs.x(),
                0  // W component remains unchanged
            };     
        }

        T& operator[](int i) {
            return this->_data[i];
        }

        const T& operator[](int i) const {
            return this->_data[i];
        }

        friend Vec4<T> operator+(const Vec4<T> lhs, const Vec4<T>& rhs) {
            Vec4<T> res;

            for (int i = 0; i < lhs.size(); ++i) {
                res[i] = lhs[i] + rhs[i];
            }

            return res;
        }

        friend Vec4<T> operator-(const Vec4<T>& lhs, const Vec4<T>& rhs) {
            Vec4<T> res;

            for (int i = 0; i < lhs.size(); ++i) {
                res[i] = lhs[i] - rhs[i];
            }

            return res;
        }

        friend std::ostream& operator<<(std::ostream& out, const Vec4<T>& vec4) {
            for (const T el : vec4._data) {
                out << el << ' ';
            }
            out << '\n';
            return out;
        }
    };


// Vec3
    template<typename T>
    class Vec3 : public Vec<T, 3> {
    public:
        Vec3()
            : Vec<T, 3>{}
        {}
        Vec3(std::initializer_list<T> list)
            : Vec<T, 3>{ list }
        {}

        constexpr T x() const { return this->_data[0]; }
        constexpr T y() const { return this->_data[1]; }
        constexpr T z() const { return this->_data[2]; }
        constexpr int size() const { return 3; }

        Vec3 cross(const Vec3& rhs) const {
            return Vec3{
                y() * rhs.z() - z() * rhs.y(),
                z() * rhs.x() - x() * rhs.z(),
                x() * rhs.y() - y() * rhs.x(),
            };     
        }

        T& operator[](int i) {
            return this->_data[i];
        }

        const T& operator[](int i) const {
            return this->_data[i];
        }

        friend Vec3<T> operator+(const Vec3<T> lhs, const Vec3<T>& rhs) {
            Vec3<T> res;

            for (int i = 0; i < lhs.size(); ++i) {
                res[i] = lhs[i] + rhs[i];
            }

            return res;
        }

        friend Vec3<T> operator-(const Vec3<T>& lhs, const Vec3<T>& rhs) {
            Vec3<T> res;

            for (int i = 0; i < lhs.size(); ++i) {
                res[i] = lhs[i] - rhs[i];
            }

            return res;
        }

        friend std::ostream& operator<<(std::ostream& out, const Vec3<T>& vec3) {
            for (const T el : vec3._data) {
                out << el << ' ';
            }
            out << '\n';
            return out;
        }
    };
}