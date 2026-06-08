#pragma once
#include "math.h"
#include <cstdint>
#include <iostream>

struct vec3 {
    float x, y, z;
    // union {
    //     struct { float x, y, z; };
    //     struct { float r, g, b; };
    // };

    vec3() : x(0), y(0), z(0) {};
    vec3(float v) : x(v), y(v), z(v) {};
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}; 
    
    vec3 operator+(const vec3& o) const { return {x + o.x, y + o.y, z + o.z}; }
    vec3& operator+=(const vec3& o) {*this = *this + o; return *this;}

    vec3 operator-(const vec3& o) const { return {x - o.x, y - o.y, z - o.z}; }
    vec3& operator-=(const vec3& o) {*this = *this - o; return *this;}

    vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
    vec3& operator*=(float s) {*this = *this * s; return *this;}

    vec3 operator/(float s) const { return {x / s, y / s, z / s}; } 
    vec3& operator/=(float s)  {*this = *this / s; return *this;}

    float length() {
        return sqrt(x * x + y * y + z * z);
    }

    vec3 normalize()
    {
        float len = length();

        if (len == 0.0f)
            return {0, 0, 0};

        return {
            x / len,
            y / len,
            z / len
        };
    };

    vec3 cross(const vec3& b) {
        return {
            y*b.z - z*b.y,
            z*b.x - x*b.z,
            x*b.y - y*b.x
        };
    };

    float dot(const vec3& b) {
        return x*b.x + y*b.y + z*b.z;
    }

    void print()
    {
        std::cout << "vec3(" << x << ", " << y << ", " << z << ")" << std::endl;
    }

};   