#pragma once

#include "main.hpp"

class Surface
{
public:

    Surface(VulkanContext &ctx);
    ~Surface();

private:
    VulkanContext                   &ctx;
    VkInstance                      instance;
    VkSurfaceKHR                    surface;
};
