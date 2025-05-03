#pragma once

#include "vec.hpp"
#include "matrix.hpp"
#include <cmath>
#include <concepts>
#include <iostream>

namespace my_gl {
    namespace math {
        template<std::floating_point T>
        struct Quaternion
        {
            T s;    // scalar part, s
            T x;    // vector part (x, y, z)
            T y;
            T z;

            // ctors
            Quaternion() : s(0), x(0), y(0), z(0) {}
            Quaternion(T s, T x, T y, T z) : s(s), x(x), y(y), z(z) {}
            Quaternion(const Vec3<T>& axis, T angle);       // rot axis & angle (radian)

            // util functions
            void        set(T s, T x, T y, T z);
            void        set(const Vec3<T>& axis, float angle);  // half angle (radian)
            T           length() const;                         // compute norm of q
            Quaternion& normalize();                            // convert it to unit q
            Quaternion& conjugate();                            // convert it to conjugate
            Quaternion& invert();                               // convert it to inverse q
            Matrix44<T> getMatrix() const;                      // return as 4x4 matrix
            Vec3<T>     getVector() const;                      // return as Vec3<T>

            // operators
            Quaternion  operator-() const;                      // unary operator (negate)
            Quaternion  operator+(const Quaternion& rhs) const; // addition
            Quaternion  operator-(const Quaternion& rhs) const; // subtraction
            Quaternion  operator*(T a) const;               // scalar multiplication
            Quaternion  operator*(const Quaternion& rhs) const; // multiplication
            Quaternion  operator*(const Vec3<T>& v) const;      // conjugation for rotation
            Quaternion& operator+=(const Quaternion& rhs);      // addition
            Quaternion& operator-=(const Quaternion& rhs);      // subtraction
            Quaternion& operator*=(T a);                    // scalar multiplication
            Quaternion& operator*=(const Quaternion& rhs);      // multiplication
            bool        operator==(const Quaternion& rhs) const;// exact comparison
            bool        operator!=(const Quaternion& rhs) const;// exact comparison

            // friend functions
            friend Quaternion operator*(const T a, const Quaternion q);
            friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);

            // static functions
            // find quaternion for rotating from v1 to v2
            static Quaternion getQuaternion(const Vec3<T>& v1, const Vec3<T>& v2);
            // return quaternion from Euler angles (x, y) or (x, y, z)
            // The rotation order is x->y->z.
            static Quaternion getQuaternion(const VecBase<T, 2>& angles);
            static Quaternion getQuaternion(const Vec3<T>& angles);
        };



        ///////////////////////////////////////////////////////////////////////////////
        // inline functions for Quaternion
        ///////////////////////////////////////////////////////////////////////////////
 
        template<std::floating_point T>
        inline Quaternion<T>::Quaternion(const Vec3<T>& axis, T angle)
        {
            // angle is radian
            set(axis, angle);
        }


        template<std::floating_point T>
        inline void Quaternion<T>::set(T s, T x, T y, T z)
        {
            this->s = s;  this->x = x;  this->y = y;  this->z = z;
        }


        template<std::floating_point T>
        inline void Quaternion<T>::set(const Vec3<T>& axis, float angle)
        {
            // use only half angle because of double multiplication, qpq*,
            // q at the front and its conjugate at the back
            Vec3<T> v = axis;
            v.normalize();                  // convert to unit vector
            float sine = sinf(angle);       // angle is radian
            s = cosf(angle);
            x = v.x * sine;
            y = v.y * sine;
            z = v.z * sine;
        }


        template<std::floating_point T>
        inline T Quaternion<T>::length() const
        {
            return sqrtf(s*s + x*x + y*y + z*z);
        }


        template<std::floating_point T>
        inline Quaternion<T>& Quaternion<T>::normalize()
        {
            constexpr float EPSILON = 0.00001f;
            float d = s*s + x*x + y*y + z*z;
            if(d < EPSILON)
                return *this; // do nothing if it is zero

            float invLength = 1.0f / sqrtf(d);
            s *= invLength;  x *= invLength;  y *= invLength;  z *= invLength;
            return *this;
        }


        template<std::floating_point T>
        inline Quaternion<T>& Quaternion<T>::conjugate()
        {
            x = -x;  y = -y;  z = -z;
            return *this;
        }


        template<std::floating_point T>
        inline Quaternion<T>& Quaternion<T>::invert()
        {
            constexpr float EPSILON = 0.00001f;
            float d = s*s + x*x + y*y + z*z;
            if(d < EPSILON)
                return *this; // do nothing if it is zero

            Quaternion q = *this;
            *this = q.conjugate() * (1.0f / d); // q* / |q||q|
            return *this;
        }

        template<std::floating_point T>
        inline Matrix44<T> Quaternion<T>::getMatrix() const
        {
            // NOTE: assume the quaternion is unit length
            // compute common values
            T x2  = x + x;
            T y2  = y + y;
            T z2  = z + z;
            T xx2 = x * x2;
            T xy2 = x * y2;
            T xz2 = x * z2;
            T yy2 = y * y2;
            T yz2 = y * z2;
            T zz2 = z * z2;
            T sx2 = s * x2;
            T sy2 = s * y2;
            T sz2 = s * z2;

            // build 4x4 matrix (column-major) and return
            return Matrix44<T>{
                        1 - (yy2 + zz2),  xy2 + sz2,        xz2 - sy2,        0, // column 0
                        xy2 - sz2,        1 - (xx2 + zz2),  yz2 + sx2,        0, // column 1
                        xz2 + sy2,        yz2 - sx2,        1 - (xx2 + yy2),  0, // column 2
                        0,                0,                0,                1  // column 3
            };

            // for non-unit quaternion
            // ss+xx-yy-zz, 2xy+2sz,     2xz-2sy,     0
            // 2xy-2sz,     ss-xx+yy-zz, 2yz-2sx,     0
            // 2xz+2sy,     2yz+2sx,     ss-xx-yy+zz, 0
            // 0,           0,           0,           1
        }


        template<std::floating_point T>
        inline Vec3<T> Quaternion<T>::getVector() const
        {
            return Vec3<T>{ x, y, z };
        }


        template<std::floating_point T>
        inline Quaternion<T> Quaternion<T>::operator-() const
        {
            return Quaternion<T>(-s, -x, -y, -z);
        }


        template<std::floating_point T>
        inline Quaternion<T> Quaternion<T>::operator+(const Quaternion<T>& rhs) const
        {
            return Quaternion<T>(s + rhs.s, x + rhs.x, y + rhs.y, z + rhs.z);
        }


        template<std::floating_point T>
        inline Quaternion<T> Quaternion<T>::operator-(const Quaternion<T>& rhs) const
        {
            return Quaternion<T>(s - rhs.s, x - rhs.x, y - rhs.y, z - rhs.z);
        }


        template<std::floating_point T>
        inline Quaternion<T> Quaternion<T>::operator*(T a) const
        {
            return Quaternion<T>(a*s, a*x, a*y, a*z);
        }


        template<std::floating_point T>
        inline Quaternion<T> Quaternion<T>::operator*(const Quaternion<T>& rhs) const
        {
            // qq' = [s,v] * [s',v'] = [(ss' - v . v'), v x v' + sv' + s'v]
            //NOTE: quaternion multiplication is not commutative
            Vec3<T> v1(x, y, z);                            // vector part of q
            Vec3<T> v2(rhs.x, rhs.y, rhs.z);                // vector part of q'

            Vec3<T> cross = v1.cross(v2);                   // v x v' (cross product)
            T dot = v1.dot(v2);                         // v . v' (inner product)
            Vec3<T> v3 = cross + (s * v2) + (rhs.s * v1);   // v x v' + sv' + s'v

            return Quaternion<T>(s * rhs.s - dot, v3.x, v3.y, v3.z);
        }


        template<std::floating_point T>
        inline Quaternion<T> Quaternion<T>::operator*(const Vec3<T>& v) const
        {
            Quaternion<T> q(0, x, y, z);
            return *this * q;
        }


        template<std::floating_point T>
        inline Quaternion<T>& Quaternion<T>::operator+=(const Quaternion<T>& rhs)
        {
            s += rhs.s;  x += rhs.x;  y += rhs.y;  z += rhs.z;
            return *this;
        }


        template<std::floating_point T>
        inline Quaternion<T>& Quaternion<T>::operator-=(const Quaternion& rhs)
        {
            s -= rhs.s;  x -= rhs.x;  y -= rhs.y;  z -= rhs.z;
            return *this;
        }


        template<std::floating_point T>
        inline Quaternion<T>& Quaternion<T>::operator*=(T a)
        {
            s *= a;  x *= a;  y *= a; z *= a;
            return *this;
        }


        template<std::floating_point T>
        inline Quaternion<T>& Quaternion<T>::operator*=(const Quaternion<T>& rhs)
        {
            *this = *this * rhs;    // q = qq'
            return *this;
        }


        template<std::floating_point T>
        inline bool Quaternion<T>::operator==(const Quaternion<T>& rhs) const
        {
            // exact comparison
            return (s == rhs.s) && (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
        }


        template<std::floating_point T>
        inline bool Quaternion<T>::operator!=(const Quaternion<T>& rhs) const
        {
            // exact comparison
            return (s != rhs.s) || (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
        }


        ///////////////////////////////////////////////////////////////////////////////
        // static functions
        ///////////////////////////////////////////////////////////////////////////////

        // find quaternion for rotating from v1 to v2
        template<std::floating_point T>
        inline Quaternion<T> Quaternion<T>::getQuaternion(const Vec3<T>& v1, const Vec3<T>& v2)
        {
            constexpr float EPSILON = 0.001f;
            const float HALF_PI = acos(-1) * 0.5f;

            // if two vectors are equal return the vector with 0 rotation
            if(v1.equal(v2, EPSILON))
            {
                return Quaternion<T>(v1, 0);
            }
            // if two vectors are opposite return a perpendicular vector with 180 angle
            else if(v1.equal(-v2, EPSILON))
            {
                Vec3<T> v;
                if(v1.x > -EPSILON && v1.x < EPSILON)       // if x ~= 0
                    v.set(1, 0, 0);
                else if(v1.y > -EPSILON && v1.y < EPSILON)  // if y ~= 0
                    v.set(0, 1, 0);
                else                                        // if z ~= 0
                    v.set(0, 0, 1);
                return Quaternion<T>(v, HALF_PI);
            }

            Vec3<T> u1 = v1;                    // convert to normal vector
            Vec3<T> u2 = v2;
            u1.normalize();
            u2.normalize();

            Vec3<T> v = u1.cross(u2);           // compute rotation axis
            float angle = acosf(u1.dot(u2));    // rotation angle
            return Quaternion<T>(v, angle * 0.5f); // half angle
        }


        // find quaternion from 3D rotation angle (ax, ay, az)
        template<std::floating_point T>
        inline Quaternion<T> Quaternion<T>::getQuaternion(const Vec3<T>& angles)
        {
            Quaternion<T> qx = Quaternion<T>(Vec3<T>(1,0,0), angles.x);   // rotate along X
            Quaternion<T> qy = Quaternion<T>(Vec3<T>(0,1,0), angles.y);   // rotate along Y
            Quaternion<T> qz = Quaternion<T>(Vec3<T>(0,0,1), angles.z);   // rotate along Z
            return qx * qy * qz;    // order: z->y->x
        }


        ///////////////////////////////////////////////////////////////////////////////
        // friend functions
        ///////////////////////////////////////////////////////////////////////////////

        // a * q (scalar * quat)
        template<std::floating_point T>
        inline Quaternion<T> operator*(const T a, const Quaternion<T>& q) {
            return Quaternion<T>(a*q.s, a*q.x, a*q.y, a*q.z);
        }

        template<std::floating_point T>
        inline std::ostream& operator<<(std::ostream& os, const Quaternion<T>& q) {
            os << "(" << q.s << ", " << q.x << ", " << q.y << ", " << q.z << ")";
            return os;
        }
    }
}

