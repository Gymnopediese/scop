#pragma once

#include "resources/Resources.hpp"

class Image;

class Buffer {
    public:
        VkBuffer        buffer;
        VkDeviceMemory  bufferMemory;
        VkDeviceSize    bufferSize;

        Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VulkanContext &ctx);
        ~Buffer();
        void copyToImage(Image &image, VkCommandPool &commandPool);
        void copyBuffer(Buffer srcBuffer, VkDeviceSize size, VkCommandPool &commandPool);
        void mapMemory(const void *memory, VkDeviceSize size);
    private:
        VulkanContext  &ctx;

};