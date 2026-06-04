#pragma once

#include "main.hpp"

class PhysicalDevice
{
public:
    VkPhysicalDevice                physicalDevice = VK_NULL_HANDLE;

    PhysicalDevice(VulkanContext &ctx);
    ~PhysicalDevice();
    
private:
    VulkanContext &ctx;
};
