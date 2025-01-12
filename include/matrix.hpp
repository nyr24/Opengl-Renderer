#pragma once
#include <array>
#include <cstdlib>
#include <utility>
#include <iostream>
#include <cassert>
#include <initializer_list>
#include "vec.hpp"

namespace my_gl_math {
    template<std::floating_point T, uint16_t ROWS, uint16_t COLS>
    class MatrixBase {
    public:
        // ctors
        constexpr MatrixBase() = default;
        constexpr explicit MatrixBase(T val) {
            _data.fill(val);
        }
        MatrixBase(std::initializer_list<T> init) {
            assert((init.size() == (ROWS * COLS)) && "init list length is not correct for Matrix initializion");
            std::copy(init.begin(), init.end(), _data.begin());
        }
        MatrixBase(const MatrixBase<T, ROWS, COLS>& rhs) = default;
        MatrixBase<T, ROWS, COLS>& operator=(const MatrixBase<T, ROWS, COLS>& rhs) = default;
        MatrixBase(MatrixBase<T, ROWS, COLS>&& rhs) = default;
        MatrixBase<T, ROWS, COLS>& operator=(MatrixBase<T, ROWS, COLS>&& rhs) = default;

        // cubic-bezier
        static constexpr MatrixBase<T, 3, 3> bezier_quad_mat() {
            return MatrixBase<T, 3, 3>{
                1.0f,   -2.0f,  1.0f,
                -2.0f,  2.0f,   0.0f,
                1.0f,   0.0f,   0.0f
            };
        }
        
        static constexpr MatrixBase<T, 4, 4> bezier_cubic_mat() {
            return MatrixBase<T, 4, 4>{
                -1.0f,  3.0f,   -3.0f,  1.0f,
                3.0f,   -6.0f,  3.0f,   0.0f,
                -3.0f,  3.0f,   0.0f,   0.0f,
                1.0f,   0.0f,   0.0f,   0.0f
            };
        }

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

        MatrixBase<T, ROWS, COLS>& transpose() {
            for (int i = 0; i < ROWS; ++i) {
                for (int j = i; j < COLS; ++j) {
                    T temp{ this->at(i, j) };
                    this->at(i, j) = this->at(j, i);
                    this->at(j, i) = temp;
                }
            }
            
            return *this;
        }

        friend MatrixBase<T, ROWS, COLS> operator*(const MatrixBase<T, ROWS, COLS>& lhs, const MatrixBase<T, ROWS, COLS>& rhs) {
            MatrixBase<T, ROWS, COLS> res;
            
            for (int r = 0; r < ROWS; ++r) {
                for (int c = 0; c < COLS; ++c) {
                    res.at(r, c) = 0;

                    for (int k = 0; k < COLS; ++k) {
                        res.at(r, c) += lhs.at(r, k) * rhs.at(k, c);
                    }
                }
            }

            return res;
        }

        template<uint32_t N_VEC>
        friend VecBase<T, N_VEC> operator*(const MatrixBase<T, ROWS, COLS>& m, const VecBase<T, N_VEC>& v) {
            static_assert(N_VEC == COLS && "can't multiply this matrix by this vector");

            VecBase<T, N_VEC> res;
            
            for (int r{ 0 }; r < ROWS; ++r) {
                for (int c{ 0 }; c < COLS; ++c) {
                    res[r] += m.at(r, c) * v[c];
                }
            }

            return res;
        }

        MatrixBase<T, ROWS, COLS>& operator*=(const MatrixBase<T, ROWS, COLS>& rhs) {
            auto res{ *this * rhs };
            *this = res;
            return *this;
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

        template<uint32_t N>
        MatrixBase<T, ROWS, COLS>& fill_row(const VecBase<T, N>& fill_with_vec, uint16_t row_index) {
            static_assert(N <= ROWS && "incompatible vector to fill with");
            if (row_index >= ROWS) {
                assert(false && "row_index parameter has a larger value than maximum count of rows of the matrix");
            #ifdef NDEBUG
                std::exit(EXIT_FAILURE);
            #endif
            }

            for (int i = 0; i < N; ++i) {
                T val = fill_with_vec[i];
                this->at(row_index, i) = val;
            }

            return *this;
        }

        template<uint32_t N>
        MatrixBase<T, ROWS, COLS>& fill_col(const VecBase<T, N>& fill_with_vec, uint16_t col_index) {
            static_assert(N <= COLS && "incompatible vector to fill with");
            if (col_index >= COLS) {
                assert(true && "col_index parameter has a larger value than maximum count of columns of the matrix");
            #ifdef NDEBUG
                std::exit(EXIT_FAILURE);
            #endif
            }

            for (int i = 0; i < N; ++i) {
                this->at(i, col_index) = fill_with_vec[i];
            }

            return *this;
        }

        template<uint32_t N>
        MatrixBase<T, ROWS, COLS>& fill_row(VecBase<T, N>&& fill_with_vec, uint16_t row_index) {
            static_assert(N <= ROWS && "incompatible vector to fill with");
            if (row_index >= ROWS) {
                assert(true && "row_index parameter has a larger value than maximum count of rows of the matrix");
            #ifdef NDEBUG
                std::exit(EXIT_FAILURE);
            #endif
            }

            for (int i = 0; i < N; ++i) {
                this->at(row_index, i) = fill_with_vec[i];
            }

            return *this;
        }

        template<uint32_t N>
        MatrixBase<T, ROWS, COLS>& fill_col(VecBase<T, N>&& fill_with_vec, uint16_t col_index) {
            static_assert(N <= COLS && "incompatible vector to fill with");
            if (col_index >= COLS) {
                assert(true && "col_index parameter has a larger value than maximum count of columns of the matrix");
            #ifdef NDEBUG
                std::exit(EXIT_FAILURE);
            #endif
            }

            for (int i = 0; i < N; ++i) {
                this->at(i, col_index) = fill_with_vec[i];
            }

            return *this;
        }

        const T* data() const { return _data.data(); }
        static constexpr uint16_t rows() { return ROWS; }
        static constexpr uint16_t cols() { return COLS; }

        // same stuff as with VecBase type
        std::array<T, ROWS * COLS> _data{};
    };


// Matrix 4x4
    template<std::floating_point T>
    class Matrix44 : public MatrixBase<T, 4, 4> {
    public:
        using MatrixBase<T, 4 ,4>::MatrixBase;
        // ctor derived from base
        Matrix44(const MatrixBase<T, 4, 4>& base)
            : MatrixBase<T, 4, 4>{ base }
        {}
        Matrix44(MatrixBase<T, 4, 4>&& base)
            : MatrixBase<T, 4, 4>{ std::move(base) }
        {}
        // assigment
        Matrix44<T>& operator=(const MatrixBase<T, 4, 4>& base_ref) {
            this->_data = base_ref._data;
            return *this;
        }
        Matrix44<T>& operator=(MatrixBase<T, 4, 4>&& base_ref) {
            this->_data = std::move(base_ref._data);
            return *this;
        }

        static constexpr Matrix44<T> identity() {
            Matrix44<T> res;

            for (int i = 0; i < 4; ++i) {
                res.at(i, i) = static_cast<T>(1.0);
            }

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
            
            /*const float angle_rad{ Global::degToRad(angle_deg) };*/
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
            constexpr std::array<my_gl_math::Global::AXIS, 3> axis_arr{
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

        static constexpr Matrix44<T> look_at(const Vec3<T>& cameraPos, const Vec3<T>& cameraTarget, const Vec3<T>& up) {
            const Vec3<T> cameraDir{ my_gl_math::Vec3<T>{ cameraPos - cameraTarget }.normalize_inplace() };
            const Vec3<T> cameraRight{ up.cross(cameraDir).normalize_inplace() };
            const Vec3<T> cameraUp{ cameraDir.cross(cameraRight).normalize_inplace() };

            Matrix44<T> lhs{ Matrix44<T>::identity()
                .fill_row(cameraRight, 0)
                .fill_row(cameraUp, 1)
                .fill_row(cameraDir, 2)
            };

            Matrix44<T> rhs{ Matrix44<T>::translation(cameraPos.negate_new()) };
            return lhs * rhs;
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
            /*const float angle_rad{ Global::degToRad(angle_deg) };*/
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
    };
} 
