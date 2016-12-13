#ifndef COLORS_H
#define COLORS_H

#include <cmath>
#include <iostream>
#include "Vectors.h"


///////////////////////////////////////////////////////////////////////////////
// 2D vector
///////////////////////////////////////////////////////////////////////////////
struct Color
{
    float r;
    float g;
    float b;

    // ctors
    Color() : r(0), g(0), b(0) {};
    Color(float x, float y, float z) : r(x), g(y), b(z) {};

    // operators
    float       length() const;
    Color&    normalize();  

    Color     operator+(const Color& rhs) const;    // add rhs
    Color     operator-(const Color& rhs) const;    // subtract rhs
    Color&    operator+=(const Color& rhs);         // add rhs and update this object
    Color&    operator-=(const Color& rhs);         // subtract rhs and update this object
    
    Color     operator*(const float scale) const;     // scale
    Color&    operator*=(const float scale);          // scale and update this object

    Color     operator/(const float scale) const;     // inverse scale
    Color&    operator/=(const float scale);          // scale and update this object

    float       operator[](int index) const;            // subscript operator v[0], v[1]
    float&      operator[](int index);                  // subscript operator v[0], v[1]

    friend std::ostream& operator<<(std::ostream& os, const Color& c);

};

///////////////////////////////////////////////////////////////////////////////
// inline functions for Colors
///////////////////////////////////////////////////////////////////////////////

inline Color Color::operator+(const Color& rhs) const {
    return Color(r+rhs.r, g+rhs.g, b+rhs.b);
}

inline Color Color::operator-(const Color& rhs) const {
    return Color(r-rhs.r, g-rhs.g, b-rhs.b);
}

inline Color& Color::operator+=(const Color& rhs) {
    r += rhs.r; g += rhs.g; b += rhs.b; return *this;
}

inline Color& Color::operator-=(const Color& rhs) {
    r -= rhs.r; g -= rhs.g; b -= rhs.b; return *this;
}

inline Color Color::operator*(const float a) const {
    return Color(r*a, g*a, b*a);
}

inline Color& Color::operator*=(const float a) {
    r *= a; g *= a; b *= a; return *this;
}

inline Color Color::operator/(const float a) const {
    return Color(r/a, g/a, b/a);
}

inline Color& Color::operator/=(const float a) {
    r /= a; g /= a; b /= a; return *this;
}

inline float Color::operator[](int index) const {
    return (&r)[index];
}

inline float& Color::operator[](int index) {
    return (&r)[index];
}

inline std::ostream& operator<<(std::ostream& os, const Color& c) {
    os << "(" << c.r << ", " << c.g << ", " << c.b << ")";
    return os;
}

inline float Color::length() const {
    return sqrtf(r*r + g*g + b*b);
}

inline Color& Color::normalize() {
    return (*this) /= this->length();
}


#endif



