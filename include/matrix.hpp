#pragma once
#include <array>
#include <concepts>
#include <cstdlib>
#include <utility>
#include <iostream>
#include <cassert>
#include <initializer_list>
#include "math.hpp"
#include "vec.hpp"

namespace my_gl {
    namespace math {
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

            const T& at(int index) const {
                assert((index >= 0 && index < (ROWS * COLS)) && "invalid indexing");
                return _data[index];
            }

            T& at(int index) {
                assert((index >= 0 && index < (ROWS * COLS)) && "invalid indexing");
                return _data[index];
            }

            T& operator[](int index) {
                assert((index >= 0 && index < (ROWS * COLS)) && "invalid indexing");
                return _data[index];
            }

            const T& operator[](int index) const {
                assert((index >= 0 && index < (ROWS * COLS)) && "invalid indexing");
                return _data[index];
            }

            MatrixBase<T, ROWS, COLS>& transpose() {
                for (size_t i = 0; i < ROWS; ++i) {
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
                for (size_t i = 0; i < ROWS; ++i) {
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

                for (size_t i = 0; i < N; ++i) {
                    T val = fill_with_vec[i];
                    this->at(row_index, i) = val;
                }

                return *this;
            }

            template<uint32_t N>
            MatrixBase<T, ROWS, COLS>& fill_col(const VecBase<T, N>& fill_with_vec, uint16_t col_index) {
                static_assert(N <= COLS && "incompatible vector to fill with");
                if (col_index >= COLS) {
                    assert(false && "col_index parameter has a larger value than maximum count of columns of the matrix");
                #ifdef NDEBUG
                    std::exit(EXIT_FAILURE);
                #endif
                }

                for (size_t i = 0; i < N; ++i) {
                    this->at(i, col_index) = fill_with_vec[i];
                }

                return *this;
            }

            template<uint32_t N>
            MatrixBase<T, ROWS, COLS>& fill_row(VecBase<T, N>&& fill_with_vec, uint16_t row_index) {
                static_assert(N <= ROWS && "incompatible vector to fill with");
                if (row_index >= ROWS) {
                    assert(false && "row_index parameter has a larger value than maximum count of rows of the matrix");
                #ifdef NDEBUG
                    std::exit(EXIT_FAILURE);
                #endif
                }

                for (size_t i = 0; i < N; ++i) {
                    this->at(row_index, i) = fill_with_vec[i];
                }

                return *this;
            }

            template<uint32_t N>
            MatrixBase<T, ROWS, COLS>& fill_col(VecBase<T, N>&& fill_with_vec, uint16_t col_index) {
                static_assert(N <= COLS && "incompatible vector to fill with");
                if (col_index >= COLS) {
                    assert(false && "col_index parameter has a larger value than maximum count of columns of the matrix");
                #ifdef NDEBUG
                    std::exit(EXIT_FAILURE);
                #endif
                }

                for (size_t i = 0; i < N; ++i) {
                    this->at(i, col_index) = fill_with_vec[i];
                }

                return *this;
            }

            const T* data() const { return _data.data(); }
            static constexpr uint16_t rows() { return ROWS; }
            static constexpr uint16_t cols() { return COLS; }

            std::array<T, ROWS * COLS> _data{};

        protected:
            static constexpr float EPSILON{0.00001f};

            T get_cofactor(T m0, T m1, T m2,
                            T m3, T m4, T m5,
                            T m6, T m7, T m8) const
            {
                return m0 * (m4 * m8 - m5 * m7) -
                    m1 * (m3 * m8 - m5 * m6) +
                    m2 * (m3 * m7 - m4 * m6);
            }

        };

    // Matrix 3x3
        template<std::floating_point T>
        class Matrix33 : public MatrixBase<T, 3, 3> {
        public:
            using MatrixBase<T, 3, 3>::MatrixBase;
            // ctor derived from base
            Matrix33(const MatrixBase<T, 3, 3>& base)
                : MatrixBase<T, 3, 3>{ base }
            {}
            Matrix33(MatrixBase<T, 3, 3>&& base)
                : MatrixBase<T, 3, 3>{ std::move(base) }
            {}
            // assigment
            Matrix33<T>& operator=(const MatrixBase<T, 3, 3>& base_ref) {
                this->_data = base_ref._data;
                return *this;
            }
            Matrix33<T>& operator=(MatrixBase<T, 3, 3>&& base_ref) {
                this->_data = std::move(base_ref._data);
                return *this;
            }

            static constexpr Matrix33<T> identity_new() {
                Matrix33<T> res;

                for (size_t i = 0; i < 3; ++i) {
                    res.at(i, i) = static_cast<T>(1.0);
                }

                return res;
            }

            Matrix33<T>& identity_inplace() {
                for (size_t i = 0; i < 3; ++i) {
                    this->at(i, i) = static_cast<T>(1.0);
                }

                return *this;
            }
    
            Matrix33<T>& invert()
            {
                auto& m{*this};

                T determinant, invDeterminant;
                T tmp[9];

                tmp[0] = m[4] * m[8] - m[5] * m[7];
                tmp[1] = m[7] * m[2] - m[8] * m[1];
                tmp[2] = m[1] * m[5] - m[2] * m[4];
                tmp[3] = m[5] * m[6] - m[3] * m[8];
                tmp[4] = m[0] * m[8] - m[2] * m[6];
                tmp[5] = m[2] * m[3] - m[0] * m[5];
                tmp[6] = m[3] * m[7] - m[4] * m[6];
                tmp[7] = m[6] * m[1] - m[7] * m[0];
                tmp[8] = m[0] * m[4] - m[1] * m[3];

                // check determinant if it is 0
                determinant = m[0] * tmp[0] + m[1] * tmp[3] + m[2] * tmp[6];
                if(std::abs(determinant) <= this->EPSILON)
                {
                    return this->identity_inplace(); // cannot inverse, make it identity matrix
                }

                // divide by the determinant
                invDeterminant = static_cast<T>(1.0) / determinant;
                m[0] = invDeterminant * tmp[0];
                m[1] = invDeterminant * tmp[1];
                m[2] = invDeterminant * tmp[2];
                m[3] = invDeterminant * tmp[3];
                m[4] = invDeterminant * tmp[4];
                m[5] = invDeterminant * tmp[5];
                m[6] = invDeterminant * tmp[6];
                m[7] = invDeterminant * tmp[7];
                m[8] = invDeterminant * tmp[8];

                return m;
            }

        };


    // Matrix 4x4
        template<std::floating_point T>
        class Matrix44 : public MatrixBase<T, 4, 4> {
        public:
            using MatrixBase<T, 4, 4>::MatrixBase;
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

            static constexpr Matrix44<T> identity_new() {
                Matrix44<T> res;

                for (size_t i = 0; i < 4; ++i) {
                    res.at(i, i) = static_cast<T>(1.0);
                }

                return res;
            }

            Matrix44<T>& identity_inplace() {
                for (size_t i = 0; i < 4; ++i) {
                    this->at(i, i) = static_cast<T>(1.0);
                }

                return *this;
            }

            static Matrix44<T> scaling(const Vec3<T>& scaling_vec) {
                Matrix44<T> scalingMatrix{ Matrix44<T>::identity_new() };
                scalingMatrix.scale(scaling_vec);
                return scalingMatrix;
            }

            static Matrix44<T> translation(const Vec3<T>& translation_vec) {
                Matrix44<T> translationMatrix{ Matrix44<T>::identity_new() };
                translationMatrix.translate(translation_vec);
                return translationMatrix;
            }

            static Matrix44<T> rotation(T angle_deg, Global::AXIS axis) {
                Matrix44<T> rotation_matrix{ Matrix44<T>::identity_new() };
                rotation_matrix.rotate(angle_deg, axis);
                return rotation_matrix;
            }

            static Matrix44<T> rotation3d(const my_gl::math::Vec3<T>& anglesVec) {
                Matrix44<T> res{ Matrix44<T>::identity_new() };
                res.rotate3d(anglesVec);
                return res;
            }
 
            static Matrix44<T> shearing(my_gl::math::Global::AXIS direction, const my_gl::math::VecBase<T, 2>& values) {
                Matrix44<T> res{ Matrix44<T>::identity_new() };
                res.shear(direction, values);
                return res;
            }

            // symmetric
            static Matrix44<T> perspective_fov(T fov_y_deg, T aspect, T zNear, T zFar) {
                const T fov_y_rad{ my_gl::math::Global::degToRad(fov_y_deg) };
                const T top_to_near{ std::tan(fov_y_rad / 2) };
                const T top{ top_to_near * zNear };
                const T right{ top * aspect };

                Matrix44<T> res;

                res.at(0, 0) = zNear / right;
                res.at(1, 1) = zNear / top;
                res.at(2, 2) = (-(zFar + zNear)) / (zFar - zNear);
                res.at(2, 3) = (-2.0f * zFar * zNear) / (zFar - zNear);
                res.at(3, 2) = -1.0f; 

                return res;
            }

            static Matrix44<T> perspective(T right, T left, T top, T bottom, T zNear, T zFar) {
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

            static Matrix44<T> look_at(const Vec3<T>& cameraPos, const Vec3<T>& cameraTarget, const Vec3<T>& up) {
                const Vec3<T> cameraDir{ my_gl::math::Vec3<T>{ cameraPos - cameraTarget }.normalize_inplace() };
                const Vec3<T> cameraRight{ up.cross(cameraDir).normalize_inplace() };
                const Vec3<T> cameraUp{ cameraDir.cross(cameraRight).normalize_inplace() };

                Matrix44<T> lhs{ Matrix44<T>::identity_new()
                    .fill_row(cameraRight, 0)
                    .fill_row(cameraUp, 1)
                    .fill_row(cameraDir, 2)
                };

                Matrix44<T> rhs{ Matrix44<T>::translation(cameraPos.negate_new()) };
                return lhs * rhs;
            }

        // non-static
            T get_determinant() const
            {
                auto& m{*this};
                return  m[0] * this->get_cofactor(m[5],m[6],m[7],m[9],m[10],m[11],m[13],m[14],m[15]) -
                    m[1] * this->get_cofactor(m[4],m[6],m[7],m[8],m[10],m[11],m[12],m[14],m[15]) +
                    m[2] * this->get_cofactor(m[4],m[5],m[7],m[8],m[9],m[11],m[12],m[13],m[15]) -
                    m[3] * this->get_cofactor(m[4],m[5],m[6],m[8],m[9],m[10],m[12],m[13],m[14]);
            }

            Matrix44<T>& invert()
            {
                auto& m{*this};
                // If the 4th row is [0,0,0,1] then it is affine matrix and
                // it has no projective transformation.
                if(m[3] == 0 && m[7] == 0 && m[11] == 0 && m[15] == 1)
                    this->invert_affine();
                else
                {
                    this->invert_general();
                    /*@@ invertProjective() is not optimized (slower than generic one)
                    if(fabs(m[0]*m[5] - m[1]*m[4]) > EPSILON)
                        this->invertProjective();   // inverse using matrix partition
                    else
                        this->invertGeneral();      // generalized inverse
                    */
                }

                return *this;
            }


            Matrix44<T>& invert_affine()
            {
                auto& m{*this};
                // R^-1
                Matrix33<T> r{ m[0],m[1],m[2],m[4],m[5],m[6],m[8],m[9],m[1] };
                r.invert();
                m[0] = r[0];  m[1] = r[1];  m[2] = r[2];
                m[4] = r[3];  m[5] = r[4];  m[6] = r[5];
                m[8] = r[6];  m[9] = r[7];  m[10]= r[8];

                // -R^-1 * T
                float x = m[12];
                float y = m[13];
                float z = m[14];
                m[12] = -(r[0] * x + r[3] * y + r[6] * z);
                m[13] = -(r[1] * x + r[4] * y + r[7] * z);
                m[14] = -(r[2] * x + r[5] * y + r[8] * z);

                // last row should be unchanged (0,0,0,1)
                //m[3] = m[7] = m[11] = 0.0f;
                //m[15] = 1.0f;

                return m;
            }

            Matrix44<T>& invert_general()
            {
                auto& m{*this};
                // get cofactors of minor matrices
                T cofactor0 = this->get_cofactor(m[5],m[6],m[7], m[9],m[10],m[11], m[13],m[14],m[15]);
                T cofactor1 = this->get_cofactor(m[4],m[6],m[7], m[8],m[10],m[11], m[12],m[14],m[15]);
                T cofactor2 = this->get_cofactor(m[4],m[5],m[7], m[8],m[9], m[11], m[12],m[13],m[15]);
                T cofactor3 = this->get_cofactor(m[4],m[5],m[6], m[8],m[9], m[10], m[12],m[13],m[14]);

                // get determinant
                T determinant = m[0] * cofactor0 - m[1] * cofactor1 + m[2] * cofactor2 - m[3] * cofactor3;
                if(std::abs(determinant) <= this->EPSILON)
                {
                    return this->identity_inplace();
                }

                // get rest of cofactors for adj(M)
                T cofactor4 = this->get_cofactor(m[1],m[2],m[3], m[9],m[10],m[11], m[13],m[14],m[15]);
                T cofactor5 = this->get_cofactor(m[0],m[2],m[3], m[8],m[10],m[11], m[12],m[14],m[15]);
                T cofactor6 = this->get_cofactor(m[0],m[1],m[3], m[8],m[9], m[11], m[12],m[13],m[15]);
                T cofactor7 = this->get_cofactor(m[0],m[1],m[2], m[8],m[9], m[10], m[12],m[13],m[14]);

                T cofactor8 = this->get_cofactor(m[1],m[2],m[3], m[5],m[6], m[7],  m[13],m[14],m[15]);
                T cofactor9 = this->get_cofactor(m[0],m[2],m[3], m[4],m[6], m[7],  m[12],m[14],m[15]);
                T cofactor10= this->get_cofactor(m[0],m[1],m[3], m[4],m[5], m[7],  m[12],m[13],m[15]);
                T cofactor11= this->get_cofactor(m[0],m[1],m[2], m[4],m[5], m[6],  m[12],m[13],m[14]);

                T cofactor12= this->get_cofactor(m[1],m[2],m[3], m[5],m[6], m[7],  m[9], m[10],m[11]);
                T cofactor13= this->get_cofactor(m[0],m[2],m[3], m[4],m[6], m[7],  m[8], m[10],m[11]);
                T cofactor14= this->get_cofactor(m[0],m[1],m[3], m[4],m[5], m[7],  m[8], m[9], m[11]);
                T cofactor15= this->get_cofactor(m[0],m[1],m[2], m[4],m[5], m[6],  m[8], m[9], m[10]);

                // build inverse matrix = adj(M) / det(M)
                // adjugate of M is the transpose of the cofactor matrix of M
                T invDeterminant = 1.0f / determinant;
                m[0] =  invDeterminant * cofactor0;
                m[1] = -invDeterminant * cofactor4;
                m[2] =  invDeterminant * cofactor8;
                m[3] = -invDeterminant * cofactor12;

                m[4] = -invDeterminant * cofactor1;
                m[5] =  invDeterminant * cofactor5;
                m[6] = -invDeterminant * cofactor9;
                m[7] =  invDeterminant * cofactor13;

                m[8] =  invDeterminant * cofactor2;
                m[9] = -invDeterminant * cofactor6;
                m[10]=  invDeterminant * cofactor10;
                m[11]= -invDeterminant * cofactor14;

                m[12]= -invDeterminant * cofactor3;
                m[13]=  invDeterminant * cofactor7;
                m[14]= -invDeterminant * cofactor11;
                m[15]=  invDeterminant * cofactor15;

                return m;
            }

            Matrix44<T>& scale(const Vec3<T>& scaling_vec) {
                this->at(0, 0) = scaling_vec.x();
                this->at(1, 1) = scaling_vec.y();
                this->at(2, 2) = scaling_vec.z();
                return *this;
            }

            Matrix44<T>& translate(const Vec3<T>& translation_vec) {
                this->at(0, 3) = translation_vec.x();
                this->at(1, 3) = translation_vec.y();
                this->at(2, 3) = translation_vec.z();
                return *this;
            }

            Matrix44<T>& rotate(T angle_deg, Global::AXIS axis) {
                const T angle_rad{ Global::degToRad(angle_deg) };
                const T angle_sin{ std::sin(angle_rad) };
                const T angle_cos{ std::cos(angle_rad) };

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

                return *this;
            }

            Matrix44<T>& rotate3d(const my_gl::math::Vec3<T>& rotationVec) {
                std::array<Matrix44<T>, 3> mat_arr;
                std::array<my_gl::math::Global::AXIS, 3> axis_arr{
                    my_gl::math::Global::X,
                    my_gl::math::Global::Y,
                    my_gl::math::Global::Z,
                };

                for (size_t i = 0; i < 3; ++i) {
                    mat_arr[i] = rotation(rotationVec[i], axis_arr[i]);
                }

                *this = std::move(mat_arr[0] * mat_arr[1] * mat_arr[2]);
                return *this;
            }

            Matrix44<T>& shear(my_gl::math::Global::AXIS direction, const my_gl::math::VecBase<T, 2>& values) {
                switch (direction) {
                case my_gl::math::Global::AXIS::X:
                    this->at(0, 1) = values[0];
                    this->at(0, 2) = values[1];
                    break;
                case my_gl::math::Global::AXIS::Y:
                    this->at(1, 0) = values[0];
                    this->at(1, 2) = values[1];
                    break;
                case my_gl::math::Global::AXIS::Z:
                    this->at(2, 0) = values[0];
                    this->at(2, 1) = values[1];
                    break;
                }

                return *this;
            }
        };

        enum class TransformationType {
            TRANSLATION,
            ROTATION,
            ROTATION3d,
            SCALING,
            SHEAR
        };

        template<std::floating_point T>
        struct Transformation {
            Matrix44<T>         _inner_mat;
            TransformationType  _transformation_type;

            static Transformation<T> scaling(const Vec3<T>& scaling_vec) {
                Transformation<T> scaling_transf = Transformation<T>{
                    ._inner_mat = Matrix44<T>::scaling(scaling_vec),
                    ._transformation_type = TransformationType::SCALING,
                };
                return scaling_transf;
            }

            static Transformation<T> translation(const Vec3<T>& translation_vec) {
                Transformation<T> translation_transf = Transformation<T>{
                    ._inner_mat = Matrix44<T>::translation(translation_vec),
                    ._transformation_type = TransformationType::TRANSLATION,
                };
                return translation_transf;
            }

            static Transformation<T> rotation3d(const Vec3<T>& rotation3d_vec) {
                Transformation<T> rotation3d_transf = Transformation<T>{
                    ._inner_mat = Matrix44<T>::rotation3d(rotation3d_vec),
                    ._transformation_type = TransformationType::ROTATION3d,
                };
                return rotation3d_transf;
            }

            static Transformation<T> rotation(T rotation_angle_deg, my_gl::math::Global::AXIS rotation_axis) {
                Transformation<T> rotation_transf = Transformation<T>{
                    ._inner_mat = Matrix44<T>::rotation(rotation_angle_deg, rotation_axis),
                    ._transformation_type = TransformationType::ROTATION
                };
                return rotation_transf;
            }

            static Transformation<T> shearing(my_gl::math::Global::AXIS shear_axis, const VecBase<T, 2>& shear_vec) {
                Transformation<T> shearing_transf = Transformation<T>{
                    ._inner_mat = Matrix44<T>::shearing(shear_axis, shear_vec),
                    ._transformation_type = TransformationType::SHEAR,
                };
                return shearing_transf;
            }
        };
    }
}
