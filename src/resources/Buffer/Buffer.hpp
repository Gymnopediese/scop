#pragma once

#include "resources/Resources.hpp"

class Image;

class Buffer {
    public:
        VkBuffer        buffer       = VK_NULL_HANDLE;
        VkDeviceMemory  bufferMemory = VK_NULL_HANDLE;
        VkDeviceSize    bufferSize;

        Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VulkanContext &ctx);
        ~Buffer();
        void copyToImage(Image &image, VkCommandPool &commandPool);
        void copyBuffer(Buffer &srcBuffer, VkDeviceSize size, VkCommandPool &commandPool);
        void mapMemory(const void *memory, VkDeviceSize size);
    private:
        VulkanContext  &ctx;

};