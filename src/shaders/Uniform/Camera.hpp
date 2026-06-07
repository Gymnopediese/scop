#pragma once

#include "main.hpp"


struct CameraBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
    glm::vec3 position;
};