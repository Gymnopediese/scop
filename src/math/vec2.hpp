#pragma once
#include <iostream>
#include <cstdint>

struct vec2 {
    float x, y;
    // union {
    //     struct { float x, y; };
    //     struct { float r, g; };
    // };

    vec2() : x(0), y(0) {}; 
    vec2(float v) : x(v), y(v) {}; 
    vec2(float x, float y) : x(x), y(y) {}; 
    vec2 operator+(const vec2& o) const { return {x + o.x, y + o.y}; }
    vec2 operator-(const vec2& o) const { return {x - o.x, y - o.y}; }
    vec2 operator*(float s) const { return {x * s, y * s}; }
    vec2 operator/(float s) const { return {x / s, y / s}; } 

    void print()
    {
        std::cout << "vec2(" << x << ", " << y << ")" << std::endl;
    }
};   