#pragma once
#include <cmath>

template<typename T>
class Vec {
public:
    Vec(): x{ 0 }, y{ 0 }, z{ 0 } {};
    Vec(T xx): x{ xx }, y{ xx }, z{ xx } {}; 
    Vec(T xx, T yy, T zz): x{ xx }, y { yy }, z{ zz } {};

    T length() const {
        return sqrt(x * x + y * y + z * z);
    }

    Vec<T>& normalize() {
        T vLength{ length() };
        if (vLength > 0) {
            T invLength{ 1 / vLength };
            x *= invLength;
            y *= invLength;
            z *= invLength;
        }
        return *this;
    }

    T x;
    T y;
    T z;
};