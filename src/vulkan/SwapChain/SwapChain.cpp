
#include "SwapChain.hpp"
#include "vulkan/vulkan_core.h"

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VulkanContext &ctx) {
    SwapChainSupportDetails details;
    
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, *ctx.surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, *ctx.surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, *ctx.surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, *ctx.surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, *ctx.surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

SwapChain::SwapChain(VulkanContext &ctx) : ctx(ctx)  {
    createSwapChain();
    std::cout << "[SWAPCHAIN] SWAPCHAIN CREATED" << std::endl;
    createImageViews();
    std::cout << "[SWAPCHAIN] IMAGE VIEWS CREATED" << std::endl;
    ctx.swapChain = this;
    std::cout << "[SWAPCHAIN] DONE" << std::endl;
}

void SwapChain::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(*ctx.physicalDevice, ctx);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, ctx);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = *ctx.surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;


    QueueFamilyIndices indices = findQueueFamilies(*ctx.physicalDevice, ctx);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optionnel
        createInfo.pQueueFamilyIndices = nullptr; // Optionnel
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(*ctx.device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("échec de la création de la swap chain!");
    }

    vkGetSwapchainImagesKHR(*ctx.device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(*ctx.device, swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;

}

void SwapChain::createImageViews() {
    swapChainImageViews.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {

        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = swapChainImages[i];
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = swapChainImageFormat;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(*ctx.device, &viewInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("échec de la creation de la vue sur une image!");
        }
    }
}




SwapChain::~SwapChain(){
    std::cout << "[Destroying] SwapChain" << std::endl;
    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(*ctx.device, imageView, nullptr);
    }
    vkDestroySwapchainKHR(*ctx.device, swapChain, nullptr);
}


// void SwapChain::Destroy()
// {
//     std::cout << "[Destroying] SwapChain" << std::endl;

//     vkDestroyDescriptorPool(*ctx.device, descriptorPool, nullptr);

//     vkDestroyCommandPool(*ctx.device, commandPool, nullptr);
//     for (auto framebuffer : swapChainFramebuffers) {
//         vkDestroyFramebuffer(*ctx.device, framebuffer, nullptr);
//     }
//     vkDestroyRenderPass(*ctx.device, renderPass, nullptr);
//     for (auto imageView : swapChainImageViews) {
//         delete imageView;
//     }
//     vkDestroySwapchainKHR(*ctx.device, swapChain, nullptr);
//     delete pipeline;
//     delete image;
// }

// SwapChain::~SwapChain(){
//     Destroy();
//     delete vertexBuffer;
//     delete indexBuffer;
//     vkDestroyDescriptorPool(*ctx.device, descriptorPool, nullptr);
//     vkDestroyDescriptorSetLayout(*ctx.device, descriptorSetLayout, nullptr);
// }