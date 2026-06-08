#include "ImageView.hpp"

ImageView::ImageView(VkImage &image, VkFormat format, VkImageAspectFlags aspectFlags, VulkanContext &ctx) : ctx(ctx) {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;
    viewInfo.subresourceRange.aspectMask = aspectFlags;

    if (vkCreateImageView(*ctx.device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("échec de la creation de la vue sur une image!");
    }
    std::cout << imageView << std::endl;
}


ImageView::~ImageView()
{

    std::cout << "[Destroying] Image View" << std::endl;
    vkDestroyImageView(*ctx.device, imageView, nullptr);
}