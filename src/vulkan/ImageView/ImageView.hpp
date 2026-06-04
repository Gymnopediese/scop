#pragma once

#include "main.hpp"

class ImageView
{
public:
    VkImageView                imageView;

    ImageView(VkImage &image, VkFormat format, VulkanContext &ctx);
    ~ImageView();
    
private:
    VulkanContext &ctx;
    
};
