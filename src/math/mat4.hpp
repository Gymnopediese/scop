#pragma once

#include "vec3.hpp"


struct mat4 {

    float m[4][4]; // m[column][row]

    mat4(){
        for (int x = 0; x < 4; x += 1)
            for (int y= 0; y < 4; y += 1)
                m[x][y] = 0;
    }; 

    mat4(float o[4][4]){
        for (int x = 0; x < 4; x += 1)
            for (int y= 0; y < 4; y += 1)
                m[x][y] = o[x][y];
    }; 

    mat4(float v){
        for (int x = 0; x < 4; x += 1)
            for (int y= 0; y < 4; y += 1)
                m[x][y] = v;
    }; 

    mat4& operator=(const mat4& other) {
        if (this == &other) return *this;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m[i][j] = other.m[i][j];
            }
        }
        return *this;
    }

    mat4 operator+(const mat4& o)const;
    mat4 operator-(const mat4& o)const;

    mat4 operator*(const mat4& b) const {
        mat4 r;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                r.m[i][j] =
                    m[i][0] * b.m[0][j] +
                    m[i][1] * b.m[1][j] +
                    m[i][2] * b.m[2][j] +
                    m[i][3] * b.m[3][j];
            }
        }

        return r;
    }
    mat4 operator/(float s) const ;

    float (&operator[](int i))[4] {
        return m[i];
    };


    static mat4 identity() {
        mat4 r = mat4();
        r.m[0][0] = 1;
        r.m[1][1] = 1;
        r.m[2][2] = 1;
        r.m[3][3] = 1;
        return r;
    }

    static mat4 perspective(float fov, float aspect, float near, float far) {
        mat4 r  = mat4();

        float t = tan(fov * 0.5f);

        r.m[0][0] = 1.0f / (aspect * t);
        r.m[1][1] = 1.0f / t;

        r.m[2][2] = -(far + near) / (far - near);
        r.m[3][2] = -(2.0f * far * near) / (far - near);

        r.m[2][3] = -1.0f;
        r.m[3][3] = 0.0f;

        return r;
    }


    static mat4 lookAt(vec3 eye, vec3 center, vec3 up) {
        vec3 f = (center - eye).normalize();     // forward
        vec3 s = f.cross(up).normalize();     // right
        vec3 u = f.cross(s);                 // corrected up

        mat4 r = mat4();

        r.m[0][0] = s.x;
        r.m[1][0] = s.y;
        r.m[2][0] = s.z;
        r.m[3][0] = -s.dot(eye);

        r.m[0][1] = u.x;
        r.m[1][1] = u.y;
        r.m[2][1] = u.z;
        r.m[3][1] = -u.dot(eye);

        r.m[0][2] = -f.x;
        r.m[1][2] = -f.y;
        r.m[2][2] = -f.z;
        r.m[3][2] = f.dot(eye);

        r.m[0][3] = 0;
        r.m[1][3] = 0;
        r.m[2][3] = 0;
        r.m[3][3] = 1;

        return r;
    }


    void print()
    {
            for (int x = 0; x < 4; x += 1)
        {
            for (int y = 0; y < 4; y += 1)
            {
                std::cout << m[x][y] << " ";
            }
            std::cout << std::endl;
        }
        
    }
};   



// mat4 lookAt(vec3 eye, vec3 center, vec3 up);