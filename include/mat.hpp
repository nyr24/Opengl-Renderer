#pragma once
#include <array>
#include <utility>
#include <iostream>
#include <cassert>
#include "vec.hpp"
#include "global.hpp"

namespace my_gl_math {
    template<typename T, int ROWS, int COLS>
    class MatrixBase {
    public:
        MatrixBase() = default;
        explicit MatrixBase(T val) {
            _data.fill(val);
        }
        MatrixBase(const MatrixBase<T, ROWS, COLS>& rhs) = default;
        MatrixBase& operator=(const MatrixBase<T, ROWS, COLS>& rhs) = default;
        MatrixBase(MatrixBase<T, ROWS, COLS>&& rhs) = default;
        MatrixBase& operator=(MatrixBase<T, ROWS, COLS>&& rhs) = default;

        const T& at(int row, int col) const {
            const int index{ row * COLS + col };
            assert((index >= 0 && index < (ROWS * COLS)) && "invalid indexing");
            return _data[row * COLS + col];
        }

        T& at(int row, int col) {
            return const_cast<T&>(std::as_const(*this).at(row, col));
        }

        void print() const {
            for (int i = 0; i < ROWS; ++i) {
                for (int j = 0; j < COLS; ++j) {
                    std::cout << at(i, j) << ' ';
                }
                std::cout << '\n';
            }
        }

        const T* data() const { return _data.data(); }

    private:
        std::array<T, ROWS * COLS> _data{};
    };


// Matrix 4x4
    template<typename T = float>
    class Matrix44 : public MatrixBase<T, 4, 4> {
    public:
        using MatrixBase<T, 4 ,4>::MatrixBase;

        static Matrix44 identity() {
            Matrix44 res;

            res.at(0, 0) = 1;
            res.at(1, 1) = 1;
            res.at(2, 2) = 1;
            res.at(3, 3) = 1;

            return res;
        }

        static Matrix44 scaling(const Vec3<T>& scalingVec) {
            Matrix44 scalingMatrix{ Matrix44::identity() };
            
            scalingMatrix.at(0, 0) = scalingVec.x();
            scalingMatrix.at(1, 1) = scalingVec.y();
            scalingMatrix.at(2, 2) = scalingVec.z();

            return scalingMatrix;
        }

        static Matrix44 translation(const Vec3<T>& translationVec) {
            Matrix44 translationMatrix{ Matrix44::identity() };

            translationMatrix.at(0, 3) = translationVec.x();
            translationMatrix.at(1, 3) = translationVec.y();
            translationMatrix.at(2, 3) = translationVec.z();

            return translationMatrix;
        }

        static Matrix44 rotation(float angleDeg, Global::AXIS axis) {
            Matrix44 rotationMatrix{ Matrix44::identity() };
            
            const float angleRad{ angleDeg * Global::degToRad(angleDeg) };
            const float angleSin{ sinf(angleDeg) };
            const float angleCos{ cosf(angleDeg) };

            switch (axis) {
            case Global::AXIS::X:
                rotationMatrix.at(1, 1) = angleCos;
                rotationMatrix.at(1, 2) = -angleSin;
                rotationMatrix.at(2, 1) = angleSin;
                rotationMatrix.at(2, 2) = angleCos;
                break;
            case Global::AXIS::Y:
                rotationMatrix.at(0, 0) = angleCos;
                rotationMatrix.at(0, 2) = angleSin;
                rotationMatrix.at(2, 0) = -angleSin;
                rotationMatrix.at(2, 2) = angleCos;
                break;
            case Global::AXIS::Z:
                rotationMatrix.at(0, 0) = angleCos;
                rotationMatrix.at(0, 1) = -angleSin;
                rotationMatrix.at(1, 0) = angleSin;
                rotationMatrix.at(1, 1) = angleCos;
                break;
            }

            return rotationMatrix;
        }

        static Matrix44 rotation3d(const my_gl_math::Vec3<float>& anglesVec) {
            std::array<Matrix44, 3> mat_arr;
            std::array<my_gl_math::Global::AXIS, 3> axis_arr{
                my_gl_math::Global::X,
                my_gl_math::Global::Y,
                my_gl_math::Global::Z,
            };
            
            for (int i = 0; i < 3; ++i) {
                mat_arr[i] = std::move(my_gl_math::Matrix44<float>::rotation(anglesVec[i], axis_arr[i]));
            }

            return Matrix44{ mat_arr[0] * mat_arr[1] * mat_arr[2] };
        }
        
    // symmetric
        static Matrix44 perspective(float fovYdeg, float aspect, float zFar, float zNear) {
            const float fovYRad{ my_gl_math::Global::degToRad(fovYdeg) };
            const float topToNear{ tanf(fovYRad / 2) };
            const float top{ topToNear * zNear };
            const float right{ top * aspect };

            Matrix44 res;

            res.at(0, 0) = zNear / right;
            res.at(1, 1) = zNear / top;
            res.at(2, 2) = (-(zFar + zNear)) / (zFar - zNear);
            res.at(2, 3) = (-2.0f * zFar * zNear) / (zFar - zNear);
            res.at(3, 2) = -1.0f; 

            return res;
        }

        static Matrix44 perspective(float right, float left, float top, float bottom, float zNear, float zFar) {
            Matrix44 res;

            res.at(0, 0) = (2.0f * zNear) / (right - left);
            res.at(0, 2) = (right + left) / (right - left);
            res.at(1, 1) = (2.0f * zNear) / (top - bottom);
            res.at(1, 2) = (top + bottom) / (top - bottom);
            res.at(2, 2) = (-(zFar + zNear)) / (zFar - zNear);
            res.at(2, 3) = (-2.0f * zFar * zNear) / (zFar - zNear);
            res.at(3, 2) = -1.0f;  

            return res;
        }

    // non-static
        void scale(const Vec3<T>& scalingVec) {
            this->at(0, 0) = scalingVec.x();
            this->at(1, 1) = scalingVec.y();
            this->at(2, 2) = scalingVec.z();
        }

        void translate(const Vec3<T>& translationVec) {
            this->at(0, 3) = translationVec.x();
            this->at(1, 3) = translationVec.y();
            this->at(2, 3) = translationVec.z();
        }

        void rotate(float angleDeg, Global::AXIS axis) {
            const float angleRad{ angleDeg * Global::degToRad(angleDeg) };
            const float angleSin{ sinf(angleDeg) };
            const float angleCos{ cosf(angleDeg) };

            switch (axis) {
            case Global::AXIS::X:
                this->at(1, 1) = angleCos;
                this->at(1, 2) = -angleSin;
                this->at(2, 1) = angleSin;
                this->at(2, 2) = angleCos;
                break;
            case Global::AXIS::Y:
                this->at(0, 0) = angleCos;
                this->at(0, 2) = angleSin;
                this->at(2, 0) = -angleSin;
                this->at(2, 2) = angleCos;
                break;
            case Global::AXIS::Z:
                this->at(0, 0) = angleCos;
                this->at(0, 1) = -angleSin;
                this->at(1, 0) = angleSin;
                this->at(1, 1) = angleCos;
                break;
            }
        }

        void rotate3d(const my_gl_math::Vec3<float>& rotationVec) {
            std::array<Matrix44<float>, 3> mat_arr;
            std::array<my_gl_math::Global::AXIS, 3> axis_arr{
                my_gl_math::Global::X,
                my_gl_math::Global::Y,
                my_gl_math::Global::Z,
            };

            for (int i = 0; i < 3; ++i) {
                mat_arr[i] = std::move(rotation(rotationVec[i], axis_arr[i]));
            }

            *this = std::move(mat_arr[0] * mat_arr[1] * mat_arr[2]);
        }


    // need to be tested
        Matrix44& transpose() {
            for (int i = 1; i < ROW_COUNT; ++i) {
                std::swap(this->at(i, 0), this->at(0, i));
            }

            for (int i = 2; i < ROW_COUNT; ++i) {
                std::swap(this->at(i, 1), this->at(1, i));
            }
            
            std::swap(this->at(3, 2), this->at(2, 3));
            
            return *this;
        }

    // operators
        friend Matrix44<T> operator*(const Matrix44<T>& lhs, const Matrix44<T>& rhs) {
            Matrix44<T> res;
            
            for (int r = 0; r < lhs.ROW_COUNT; ++r) {
                for (int c = 0; c < lhs.COL_COUNT; ++c) {
                    res.at(r, c) = 0;

                    for (int k = 0; k < lhs.COL_COUNT; ++k) {
                        res.at(r, c) += lhs.at(r, k) * rhs.at(k, c);
                    }
                }
            }

            return res;
        }

        friend Vec4<T> operator*(const Matrix44<T>& m, const Vec4<T>& v) {
            Vec4<T> res;
            
            for (int r{ 0 }; r < m.ROW_COUNT; ++r) {
                for (int c{ 0 }; c < v.size(); ++c) {
                    res[r] += m.at(r, c) * v[c];
                }
            }

            return res;
        }
        
        friend Vec3<T> operator*(const Matrix44<T>& m, const Vec3<T>& v) {
            Vec3<T> res;

            for (int r{ 0 }; r < m.ROW_COUNT; ++r) {
                for (int c{ 0 }; c < v.size(); ++c) {
                    res[r] += m.at(r, c) * v[c];
                }
            }

            return res;
        }

        Matrix44<T>& operator*=(const Matrix44<T>& rhs) {
            for (int r = 0; r < ROW_COUNT; ++r) {
                for (int c = 0; c < COL_COUNT; ++c) {
                    this->at(r, c) = 0;

                    for (int k = 0; k < COL_COUNT; ++k) {
                        this->at(r, c) += this->at(r, k) * rhs.at(k, c);
                    }
                }
            }

            return *this;
        }

        static constexpr int ROW_COUNT = 4;
        static constexpr int COL_COUNT = 4;
    };
} 