
#pragma once

#include "scene/Scene.hpp"


class Camera3D {
    public:

        std::vector<Buffer*>             uniformBuffers;
        std::vector<VkDescriptorSet*>    descriptorSet;

        vec3 position{0.0f, 0.0f, 10.0f};
        float yaw   = -90.0f;
        float pitch = 0.0f;

        float speed = 3.f;
        float sensitivity = 3.f;


        Camera3D(VulkanContext &ctx);
        ~Camera3D();
        void update(uint32_t currentImage, float delta);
        void processMouse(float x, float y, float delta);
        mat4 getView();
        
    private:
        VulkanContext &ctx;
};