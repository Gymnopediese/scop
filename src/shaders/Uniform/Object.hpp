
#pragma once

#include "main.hpp"

struct Transform3D
{
    alignas(16) mat4 model;
    alignas(16) int      use_texture = 1;
    alignas(16) int      shadow = 1;
};