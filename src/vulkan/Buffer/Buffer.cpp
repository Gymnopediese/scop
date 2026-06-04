
#include "Buffer.hpp"

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VulkanContext &ctx) {

    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(*ctx.physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("aucun type de memoire ne satisfait le buffer!");

}

Buffer::Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VulkanContext &ctx) : ctx(ctx) {
    bufferSize = size;
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(*ctx.device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("echec de la creation d'un buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(*ctx.device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, ctx);

    if (vkAllocateMemory(*ctx.device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("echec de l'allocation de memoire!");
    }

    vkBindBufferMemory(*ctx.device, buffer, bufferMemory, 0);
}

void Buffer::copyBuffer(Buffer srcBuffer, VkDeviceSize size, VkCommandPool &commandPool) {

    SingleUseCommandBuffer commandBuffer = SingleUseCommandBuffer(ctx, commandPool);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer.commandBuffer, srcBuffer.buffer, buffer, 1, &copyRegion);

}

void Buffer::copyToImage(VImage &image, VkCommandPool &commandPool) {
    SingleUseCommandBuffer commandBuffer = SingleUseCommandBuffer(ctx, commandPool);

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        image.width,
        image.height,
        1
    };

    vkCmdCopyBufferToImage(
        commandBuffer.commandBuffer,
        buffer,
        image.textureImage,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );


}

void Buffer::mapMemory(const void *memory, VkDeviceSize size)
{
    void* data;
    vkMapMemory(*ctx.device, bufferMemory, 0, size, 0, &data);
    memcpy(data, memory, static_cast<size_t>(size));
    vkUnmapMemory(*ctx.device, bufferMemory);
}

Buffer::~Buffer()
{
    vkDestroyBuffer(*ctx.device, buffer, nullptr);
    vkFreeMemory(*ctx.device, bufferMemory, nullptr);
}
