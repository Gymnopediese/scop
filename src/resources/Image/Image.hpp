#pragma once

#include "resources/Resources.hpp"

class ImageView;

class Image {

    public:

        VkImage         textureImage;
        VkDeviceMemory  textureImageMemory;
        ImageView       *textureImageView;
        VkFormat        format;

        u32             width;
        u32             height;


        static Image *loadPPM(const std::string& path, VulkanContext &ctx);
        Image(int width, int height, VulkanContext &ctx, int usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VkFormat format = VK_FORMAT_R8G8B8A8_SRGB);
        void transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void createTextureImageView();
        ~Image();
    private:
        VulkanContext &ctx;
        
};