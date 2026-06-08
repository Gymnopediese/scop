#pragma once

#include "main.hpp"


struct CameraBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 position;
};