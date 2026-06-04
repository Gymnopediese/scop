#pragma once

#include "main.hpp"

class VImage;

class Buffer {
    public:
        VkBuffer        buffer;
        VkDeviceMemory  bufferMemory;
        VkDeviceSize    bufferSize;

        Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VulkanContext &ctx);
        ~Buffer();
        void copyToImage(VImage &image, VkCommandPool &commandPool);
        void copyBuffer(Buffer srcBuffer, VkDeviceSize size, VkCommandPool &commandPool);
        void mapMemory(const void *memory, VkDeviceSize size);
    private:
        VulkanContext  &ctx;

};