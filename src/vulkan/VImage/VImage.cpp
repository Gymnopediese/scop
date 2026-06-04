
#include "VImage.hpp"



VImage::VImage(const std::string& path, VulkanContext &ctx, VkCommandPool &commandPool) : ctx(ctx), commandPool(commandPool){
    Image image;
    image.loadPPM(path);

    VkDeviceSize imageSize = image.width * image.height * 4;

    width = image.width;
    height = image.height;


    Buffer stagingBuffer = Buffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, ctx);


    stagingBuffer.mapMemory(image.pixels, imageSize);

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(image.width);
    imageInfo.extent.height = static_cast<uint32_t>(image.height);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;

    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;

    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0; // Optionnel

    if (vkCreateImage(*ctx.device, &imageInfo, nullptr, &textureImage) != VK_SUCCESS) {
        throw std::runtime_error("echec de la creation d'une image!");
    }


    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(*ctx.device, textureImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, ctx);

    if (vkAllocateMemory(*ctx.device, &allocInfo, nullptr, &textureImageMemory) != VK_SUCCESS) {
        throw std::runtime_error("echec de l'allocation de la mémoire pour l'image!");
    }

    vkBindImageMemory(*ctx.device, textureImage, textureImageMemory, 0);

    transitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    stagingBuffer.copyToImage(*this, commandPool);
    transitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    
    createTextureImageView();
    createTextureSampler();
}

void VImage::createTextureImageView() {
    textureImageView = new ImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB, ctx);



}

void VImage::createTextureSampler()
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;

    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = 16.0f;
    
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;

    samplerInfo.unnormalizedCoordinates = VK_FALSE;

    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;


    // samplerInfo.anisotropyEnable = VK_FALSE;
    // samplerInfo.maxAnisotropy = 1.0f;

    if (vkCreateSampler(*ctx.device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
        throw std::runtime_error("échec de la creation d'un sampler!");
    }



}

void VImage::transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
    SingleUseCommandBuffer commandBuffer = SingleUseCommandBuffer(ctx, commandPool);


    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;

    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;


    barrier.image = textureImage;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        throw std::invalid_argument("transition d'orgisation non supportée!");
    }

    vkCmdPipelineBarrier(
        commandBuffer.commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

}

VImage::~VImage(){

    vkDestroySampler(*ctx.device, textureSampler, nullptr);
    delete textureImageView;
    vkDestroyImage(*ctx.device, textureImage, nullptr);
    vkFreeMemory(*ctx.device, textureImageMemory, nullptr);

}