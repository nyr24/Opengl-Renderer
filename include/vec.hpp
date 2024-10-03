#pragma once
#include <cmath>
#include <array>
#include <initializer_list>

namespace my_gl_math {
    template<typename T, int size>
    class Vec {
    public:
        Vec() = default;
        explicit Vec(T xx) { _data.fill(xx); }
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
        T getSize() const { return _data.size(); }

        void print() const {
            for (const T el : _data) {
                std::cout << el << ' ';
            } 
            std::cout << '\n';
        }

        bool is_eq(const Vec<T, size>& rhs) {
            return _data == rhs._data;
        }

    protected:
        std::array<T, size> _data{};
    };


// Vec4
    template<typename T = float>
    class Vec4 : public Vec<T, 4> {
    public:
        using Vec<T, 4>::Vec;
        using Vec<T, 4>::operator=;

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

        // operators
        T& operator[](int i) {
            assert((i >= 0 && i < this->getSize()) && "invalid indexing");
            return this->_data[i];
        }

        const T& operator[](int i) const {
            assert((i >= 0 && i < this->getSize()) && "invalid indexing");
            return this->_data[i];
        }

        Vec4<T> operator+(const Vec4<T>& rhs) {
            Vec4<T> res;

            for (int i = 0; i < this->getSize(); ++i) {
                res[i] = this->_data[i] + rhs[i];
            }

            return res;
        }

        Vec4<T> operator-(const Vec4<T>& rhs) {
            Vec4<T> res;

            for (int i = 0; i < this->getSize(); ++i) {
                res[i] = this->_data[i] - rhs[i];
            }

            return res;
        }

        Vec4<T>& operator*(T rhs) {
            for (const T& el : this->_data) {
                el *= rhs;
            }

            return *this;
        }

        friend std::ostream& operator<<(std::ostream& out, const Vec4<T>& vec) {
            for (const T el : vec._data) {
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
        using Vec<T, 3>::Vec;
        using Vec<T, 3>::operator=;

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

    // operators
        T& operator[](int i) {
            assert((i >= 0 && i < this->getSize()) && "invalid indexing");
            return this->_data[i];
        }

        const T& operator[](int i) const {
            assert((i >= 0 && i < this->getSize()) && "invalid indexing");
            return this->_data[i];
        }

        Vec3<T> operator+(const Vec3<T>& rhs) {
            Vec3<T> res;

            for (int i = 0; i < this->getSize(); ++i) {
                res[i] = this->_data[i] + rhs[i];
            }

            return res;
        }

        Vec3<T> operator-(const Vec3<T>& rhs) {
            Vec3<T> res;

            for (int i = 0; i < this->getSize(); ++i) {
                res[i] = this->_data[i] - rhs[i];
            }

            return res;
        }

    // operations with primitives

        Vec3<T> operator*(T rhs) const {
            Vec3<T> res;
            res *= rhs;

            return *this;
        }

        Vec3<T>& operator*=(T rhs) {
            for (T& el : this->_data) {
                el *= rhs;
            }

            return *this;
        }

        friend std::ostream& operator<<(std::ostream& out, const Vec3<T>& vec) {
            for (const T el : vec._data) {
                out << el << ' ';
            }
            out << '\n';
            return out;
        }
    };
}