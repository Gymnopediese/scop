#pragma once

#include "main.hpp"

class SingleUseCommandBuffer
{
public:
    VkCommandBuffer                commandBuffer;

    SingleUseCommandBuffer(VulkanContext &ctx, VkCommandPool &commandPool);
    ~SingleUseCommandBuffer();
    
private:
    VulkanContext &ctx;
    VkCommandPool &commandPool;
};
