#pragma once

#include "main.hpp"

class ImageView;

class VImage {

    public:

        VkImage         textureImage;
        VkDeviceMemory  textureImageMemory;
        ImageView       *textureImageView;
        VkSampler       textureSampler;


        u32             width;
        u32             height;


        VImage(const std::string& path, VulkanContext &ctx, VkCommandPool &commandPool);
        void transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void createTextureImageView();
        void createTextureSampler();
        ~VImage();
    private:
        VulkanContext &ctx;
        VkCommandPool &commandPool;

        
};