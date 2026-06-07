#pragma once

#include "assets/Assets.hpp"


class Texture;

class Material {
    public:
        Pipeline                *pipeline;

        VkDescriptorSet         descriptorSet;
        VkDescriptorSetLayout   descriptorSetLayout;

        Texture*                albedo;
        Texture*                normal;

        Material(VulkanContext &ctx);
        ~Material();

    private:
        VulkanContext &ctx;

};