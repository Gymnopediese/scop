#pragma once

#include "resources/Resources.hpp"

class ImageView
{
public:
    VkImageView                imageView;

    ImageView(VkImage &image, VkFormat format, VkImageAspectFlags aspectFlags, VulkanContext &ctx);
    ~ImageView();
    
private:
    VulkanContext &ctx;
    
};
