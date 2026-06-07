
#pragma once

#include "scene/Scene.hpp"


class Camera3D {
    public:

        std::vector<Buffer*>             uniformBuffers;
        std::vector<VkDescriptorSet*>    descriptorSet;

        glm::vec3 position{0.0f, 0.0f, 5.0f};
        float yaw   = -90.0f;
        float pitch = 0.0f;

        float speed = 0.1f;
        float sensitivity = 0.1f;


        Camera3D(VulkanContext &ctx);
        ~Camera3D();
        void update(uint32_t currentImage);
        void processMouse(float x, float y);
        glm::mat4 getView();
        
    private:
        VulkanContext &ctx;
};