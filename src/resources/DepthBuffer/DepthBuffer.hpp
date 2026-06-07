
#pragma once

#include "resources/Resources.hpp"

VkFormat findDepthFormat(VulkanContext &ctx);

class DepthBuffer 
{
    public:
        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        ImageView *depthImageView;

        DepthBuffer(VulkanContext &ctx);
        ~DepthBuffer();
    
    private:
        VulkanContext &ctx;
};