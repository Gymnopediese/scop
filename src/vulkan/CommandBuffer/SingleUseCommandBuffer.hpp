#pragma once

#include "vulkan/VulkanContext.hpp"

class SingleUseCommandBuffer
{
public:
    VkCommandBuffer                commandBuffer;

    SingleUseCommandBuffer(VulkanContext &ctx);
    ~SingleUseCommandBuffer();
    
private:
    VulkanContext &ctx;
};
