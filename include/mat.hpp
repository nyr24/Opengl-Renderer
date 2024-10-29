#pragma once
#include <array>
#include <utility>
#include <iostream>
#include <cassert>
#include "vec.hpp"
#include "global.hpp"

namespace my_gl_math {
    template<typename T, int ROWS, int COLS> requires std::floating_point<T>
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
            return _data[index];
        }

        T& at(int row, int col) {
            const int index{ row * COLS + col };
            assert((index >= 0 && index < (ROWS * COLS)) && "invalid indexing");
            return _data[index];
        }

        void print() const {
            for (int i = 0; i < ROWS; ++i) {
                for (int j = 0; j < COLS; ++j) {
                    std::cout << at(i, j) << ' ';
                }
                std::cout << '\n';
            }

            std::cout << '\n';
        }

        const T* data() const { return _data.data(); }

    private:
        std::array<T, ROWS * COLS> _data{};
    };


// Matrix 4x4
    template<typename T = float> requires std::floating_point<T>
    class Matrix44 : public MatrixBase<T, 4, 4> {
    public:
        using MatrixBase<T, 4 ,4>::MatrixBase;

        static Matrix44<T> identity() {
            Matrix44<T> res;

            res.at(0, 0) = 1;
            res.at(1, 1) = 1;
            res.at(2, 2) = 1;
            res.at(3, 3) = 1;

            return res;
        }

        static Matrix44<T> scaling(const Vec3<T>& scaling_vec) {
            Matrix44<T> scalingMatrix{ Matrix44<T>::identity() };
            
            scalingMatrix.at(0, 0) = scaling_vec.x();
            scalingMatrix.at(1, 1) = scaling_vec.y();
            scalingMatrix.at(2, 2) = scaling_vec.z();

            return scalingMatrix;
        }

        static Matrix44<T> translation(const Vec3<T>& translation_vec) {
            Matrix44<T> translationMatrix{ Matrix44<T>::identity() };

            translationMatrix.at(0, 3) = translation_vec.x();
            translationMatrix.at(1, 3) = translation_vec.y();
            translationMatrix.at(2, 3) = translation_vec.z();

            return translationMatrix;
        }

        static Matrix44<T> rotation(float angle_deg, Global::AXIS axis) {
            Matrix44<T> rotation_matrix{ Matrix44<T>::identity() };
            
            const float angle_rad{ Global::degToRad(angle_deg) };
            const float angle_sin{ sinf(angle_deg) };
            const float angle_cos{ cosf(angle_deg) };

            switch (axis) {
            case Global::AXIS::X:
                rotation_matrix.at(1, 1) = angle_cos;
                rotation_matrix.at(1, 2) = -angle_sin;
                rotation_matrix.at(2, 1) = angle_sin;
                rotation_matrix.at(2, 2) = angle_cos;
                break;
            case Global::AXIS::Y:
                rotation_matrix.at(0, 0) = angle_cos;
                rotation_matrix.at(0, 2) = angle_sin;
                rotation_matrix.at(2, 0) = -angle_sin;
                rotation_matrix.at(2, 2) = angle_cos;
                break;
            case Global::AXIS::Z:
                rotation_matrix.at(0, 0) = angle_cos;
                rotation_matrix.at(0, 1) = -angle_sin;
                rotation_matrix.at(1, 0) = angle_sin;
                rotation_matrix.at(1, 1) = angle_cos;
                break;
            }

            return rotation_matrix;
        }

        static Matrix44<T> rotation3d(const my_gl_math::Vec3<float>& anglesVec) {
            std::array<Matrix44<T>, 3> mat_arr;
            std::array<my_gl_math::Global::AXIS, 3> axis_arr{
                my_gl_math::Global::X,
                my_gl_math::Global::Y,
                my_gl_math::Global::Z,
            };
            
            for (int i = 0; i < 3; ++i) {
                mat_arr[i] = std::move(my_gl_math::Matrix44<T>::rotation(anglesVec[i], axis_arr[i]));
            }

            return Matrix44<T>{ mat_arr[0] * mat_arr[1] * mat_arr[2] };
        }
        
    // symmetric
        static Matrix44<T> perspective_fov(float fov_y_deg, float aspect, float zNear, float zFar) {
            const float fov_y_rad{ my_gl_math::Global::degToRad(fov_y_deg) };
            const float top_to_near{ tanf(fov_y_rad / 2) };
            const float top{ top_to_near * zNear };
            const float right{ top * aspect };

            Matrix44<T> res;

            res.at(0, 0) = zNear / right;
            res.at(1, 1) = zNear / top;
            res.at(2, 2) = (-(zFar + zNear)) / (zFar - zNear);
            res.at(2, 3) = (-2.0f * zFar * zNear) / (zFar - zNear);
            res.at(3, 2) = -1.0f; 

            return res;
        }

        static Matrix44<T> perspective(float right, float left, float top, float bottom, float zNear, float zFar) {
            Matrix44<T> res;

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
        void scale(const Vec3<T>& scaling_vec) {
            this->at(0, 0) = scaling_vec.x();
            this->at(1, 1) = scaling_vec.y();
            this->at(2, 2) = scaling_vec.z();
        }

        void translate(const Vec3<T>& translation_vec) {
            this->at(0, 3) = translation_vec.x();
            this->at(1, 3) = translation_vec.y();
            this->at(2, 3) = translation_vec.z();
        }

        void rotate(float angle_deg, Global::AXIS axis) {
            const float angle_rad{ Global::degToRad(angle_deg) };
            const float angle_sin{ sinf(angle_deg) };
            const float angle_cos{ cosf(angle_deg) };

            switch (axis) {
            case Global::AXIS::X:
                this->at(1, 1) = angle_cos;
                this->at(1, 2) = -angle_sin;
                this->at(2, 1) = angle_sin;
                this->at(2, 2) = angle_cos;
                break;
            case Global::AXIS::Y:
                this->at(0, 0) = angle_cos;
                this->at(0, 2) = angle_sin;
                this->at(2, 0) = -angle_sin;
                this->at(2, 2) = angle_cos;
                break;
            case Global::AXIS::Z:
                this->at(0, 0) = angle_cos;
                this->at(0, 1) = -angle_sin;
                this->at(1, 0) = angle_sin;
                this->at(1, 1) = angle_cos;
                break;
            }
        }

        void rotate3d(const my_gl_math::Vec3<float>& rotationVec) {
            std::array<Matrix44<T>, 3> mat_arr;
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
        Matrix44<T>& transpose() {
            for (int i = 0; i < ROW_COUNT; ++i) {
                for (int j = i; j < COL_COUNT; ++j) {
                    T temp{ this->at(i, j) };
                    this->at(i, j) = this->at(j, i);
                    this->at(j, i) = temp;
                }
            }
            
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
            constexpr int vec_size{ res.size() };

            for (int r{ 0 }; r < vec_size; ++r) {
                for (int c{ 0 }; c < vec_size; ++c) {
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