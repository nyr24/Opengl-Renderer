#pragma once
#include <array>
#include <utility>
#include <iostream>
#include <cassert>
#include "vec.hpp"
#include "global.hpp"

namespace my_gl_math {
    template<typename T, int rows, int cols>
    class MatrixBase {
    public:
        MatrixBase() = default;
        explicit MatrixBase(T val) {
            _data.fill(val);
        }

        const T& at(int row, int col) const {
            const int index{ row * cols + col };
            assert((index >= 0 && index < (rows * cols)) && "invalid indexing");
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

        const T* data() const { return _data.data(); }

    private:
        std::array<T, rows * cols> _data;
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
            Matrix44 rotationMatrix = Matrix44::identity();
            
            const float angleRad{ angleDeg * Global::degToRad(angleDeg) };
            const float angleSin{ sinf(angleDeg) };
            const float angleCos{ cosf(angleDeg) };

            switch (axis) {
            case Global::AXIS::x:
                rotationMatrix.at(1, 1) = angleCos;
                rotationMatrix.at(2, 1) = angleSin;
                rotationMatrix.at(1, 2) = -angleSin;
                rotationMatrix.at(2, 2) = angleCos;
                break;
            case Global::AXIS::y:
                rotationMatrix.at(0, 0) = angleCos;
                rotationMatrix.at(2, 0) = -angleSin;
                rotationMatrix.at(0, 2) = angleSin;
                rotationMatrix.at(2, 2) = angleCos;
                break;
            case Global::AXIS::z:
                rotationMatrix.at(0, 0) = angleCos;
                rotationMatrix.at(1, 0) = angleSin;
                rotationMatrix.at(0, 1) = -angleSin;
                rotationMatrix.at(1, 1) = angleCos;
                break;
            }

            return rotationMatrix;
        }

    // operations
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
                for (int c{ 0 }; c < m.COL_COUNT; ++c) {
                    res[r] += m.at(r, c) * v[c];
                }
            }

            return res;
        }

    // constants
        static constexpr int ROW_COUNT = 4;
        static constexpr int COL_COUNT = 4;
    };
} 