
#include "DepthBuffer.hpp"



bool hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VulkanContext &ctx) {
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(*ctx.physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }
    throw std::runtime_error("aucun des formats demandés n'est supporté!");
}

VkFormat findDepthFormat(VulkanContext &ctx) {
    return findSupportedFormat(
        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, ctx
    );
}

void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VulkanContext &ctx) {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(*ctx.device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("echec de la creation d'une image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(*ctx.device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, ctx);

    if (vkAllocateMemory(*ctx.device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("echec de l'allocation de la memoire d'une image!");
    }

    vkBindImageMemory(*ctx.device, image, imageMemory, 0);
}


DepthBuffer::DepthBuffer(VulkanContext &ctx) : ctx(ctx)
{
    VkFormat depthFormat = findDepthFormat(ctx);

    createImage(ctx.swapChain->swapChainExtent.width, ctx.swapChain->swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory,ctx);
    depthImageView = new ImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, ctx);
}

DepthBuffer::~DepthBuffer()
{

    std::cout << "[Destroying] Deuffer" << std::endl;
    delete depthImageView;
    vkDestroyImage(*ctx.device, depthImage, nullptr);
    vkFreeMemory(*ctx.device, depthImageMemory, nullptr);
}