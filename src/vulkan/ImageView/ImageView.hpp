#pragma once

#include "main.hpp"

class ImageView
{
public:
    VkImageView                imageView;

    ImageView(VImage &image, VkFormat format, VulkanContext &ctx);
    ~ImageView();
    
private:
    VulkanContext &ctx;
    
};
