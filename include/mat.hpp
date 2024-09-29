#pragma once
#include <array>
#include <utility>
#include <iostream>
#include <cassert>
#include "vec.hpp"
#include "global.hpp"

namespace gl_math {
    template<typename T, int rows, int cols>
    class MatrixBase {
    // operations
    public:
        template<int l_rows, int l_cols, int r_rows, int r_cols>
        friend MatrixBase<T, l_rows, r_cols> operator*(
            const MatrixBase<T, l_rows, l_cols>& lhs,
            const MatrixBase<T, r_rows, r_cols>& rhs) 
        {   
            assert(l_cols == r_rows && "can't be multiplied");
            
            MatrixBase<T, l_rows, r_cols> result;
            
            for (int r = 0; r < l_rows; ++r) {
                for (int c = 0; c < r_cols; ++c) {
                    result.at(r, c) = static_cast<T>(0);

                    for (int k = 0; k < l_cols; ++k) {
                        result.at(r, c) += lhs.at(r, k) + rhs.at(k, c);
                    }
                }
            }

            return result;
        }

    // utility
    public:
        const T& at(int row, int col) const {
            return _data[row * cols + col];
        }

        T& at(int row, int col) {
            return const_cast<T&>(std::as_const(*this).at(row, col));
        }

        void print() const {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    std::cout << at(i, j) << ' ';
                }
                std::cout << '\n';
            }
        }

    protected:
        MatrixBase(): _data{ 0 } {};
        std::array<T, rows * cols> _data;
    };


// Matrix 4x4
    template<typename T = float>
    class Matrix44 : public MatrixBase<T, 4, 4> {
    public:
        static Matrix44 identity() {
            Matrix44 res;

            res.at(0, 0) = 1;
            res.at(1, 1) = 1;
            res.at(2, 2) = 1;
            res.at(3, 3) = 1;

            return res;
        }

        static Matrix44 scaling(const Vec3<T>& scalingVec) {
            Matrix44 scalingMatrix = { Matrix44::identity() };
            
            scalingMatrix.at(0, 0) = scalingVec.x();
            scalingMatrix.at(1, 1) = scalingVec.y();
            scalingMatrix.at(2, 2) = scalingVec.z();

            return scalingMatrix;
        }

        static Matrix44 translation(const Vec3<T>& translationVec) {
            Matrix44 translationMatrix = Matrix44::identity();

            translationMatrix.at(3, 0) = translationVec.x();
            translationMatrix.at(3, 1) = translationVec.y();
            translationMatrix.at(3, 2) = translationVec.z();

            return translationMatrix;
        }

        static Matrix44 rotation(float angleDeg, Global::AXIS axis) {
            Matrix44 rotationMatrix = Matrix44::identity();
            
            const T angleRad{ angleDeg * Global::degToRad(angleDeg) };
            const T angleSin{ sin(angleDeg) };
            const T angleCos{ cos(angleDeg) };

            switch (axis) {
            case Global::AXIS::x:
                rotationMatrix.at(1, 1) = angleCos;
                rotationMatrix.at(2, 1) = -angleSin;
                rotationMatrix.at(1, 2) = angleSin;
                rotationMatrix.at(2, 2) = angleCos;
                break;
            case Global::AXIS::y:
                rotationMatrix.at(0, 0) = angleCos;
                rotationMatrix.at(2, 0) = angleSin;
                rotationMatrix.at(0, 2) = -angleSin;
                rotationMatrix.at(2, 2) = angleCos;
                break;
            case Global::AXIS::z:
                rotationMatrix.at(0, 0) = angleCos;
                rotationMatrix.at(1, 0) = -angleSin;
                rotationMatrix.at(0, 1) = angleSin;
                rotationMatrix.at(1, 1) = angleCos;
                break;
            }

            return rotationMatrix;
        }

        static constexpr int ROW_COUNT = 4;
        static constexpr int COL_COUNT = 4;

        static constexpr T rows() { return ROW_COUNT; }
        static constexpr T cols() { return COL_COUNT; }
    };
} 