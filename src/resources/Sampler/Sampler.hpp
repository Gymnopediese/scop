#pragma once

#include "resources/Resources.hpp"

class Sampler
{
    public:
        VkSampler       textureSampler;

        Sampler(VulkanContext &ctx);
        ~Sampler();
    private:
        VulkanContext &ctx;
};
