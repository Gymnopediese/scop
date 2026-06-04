#pragma once

#include "main.hpp"



class Device
{
public:
    VkQueue                         graphicsQueue;
    VkQueue                         presentQueue;
    Device(VulkanContext &ctx);
    ~Device();

private:
    VkDevice device = VK_NULL_HANDLE;
};