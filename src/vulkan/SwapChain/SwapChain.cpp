
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
    Create();
    ctx.swapChain = this;

    std::cout << "[SWAPCHAIN] DONE" << std::endl;
}


void SwapChain::Create()
{
    createSwapChain();
    std::cout << "[SWAPCHAIN] SWAPCHAIN CREATED" << std::endl;
    createImageViews();
    std::cout << "[SWAPCHAIN] IMAGE VIEWS CREATED" << std::endl;
}


    
    // VkDescriptorSetLayoutBinding uboLayoutBinding{};
    // uboLayoutBinding.binding = 0;
    // uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    // uboLayoutBinding.descriptorCount = 1;

    // uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    // uboLayoutBinding.pImmutableSamplers = nullptr; // Optionnel

    // VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    // samplerLayoutBinding.binding = 1;
    // samplerLayoutBinding.descriptorCount = 1;
    // samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    // samplerLayoutBinding.pImmutableSamplers = nullptr;
    // samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    // std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
    // VkDescriptorSetLayoutCreateInfo layoutInfo{};
    // layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    // layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    // layoutInfo.pBindings = bindings.data();

    // if (vkCreateDescriptorSetLayout(*ctx.device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
    //     throw std::runtime_error("echec de la creation d'un set de descripteurs!");
    // }




void SwapChain::ReCreate()
{

    int width = 0, height = 0;
    glfwGetFramebufferSize(ctx.window->window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(ctx.window->window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(*ctx.device);

    Destroy();
    
    createSwapChain();
    createImageViews();
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



// void SwapChain::createCommandPool() {

//     QueueFamilyIndices queueFamilyIndices = findQueueFamilies(*ctx.physicalDevice, ctx);

//     VkCommandPoolCreateInfo poolInfo{};
//     poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
//     poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
//     poolInfo.flags = 0; // Optionel
    

//     if (vkCreateCommandPool(*ctx.device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
//         throw std::runtime_error("échec de la création d'une command pool!");
//     }

//     ctx->commandPool = &commandPool;
// }

// void SwapChain::createCommandBuffers() {
//     commandBuffers.resize(swapChainFramebuffers.size());

//     VkCommandBufferAllocateInfo allocInfo{};
//     allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//     allocInfo.commandPool = commandPool;
//     allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//     allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

//     if (vkAllocateCommandBuffers(*ctx.device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
//         throw std::runtime_error("échec de l'allocation de command buffers!");
//     }

//     for (size_t i = 0; i < commandBuffers.size(); i++) {
//         VkCommandBufferBeginInfo beginInfo{};
//         beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//         beginInfo.flags = 0; // Optionnel
//         beginInfo.pInheritanceInfo = nullptr; // Optionel

//         if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
//             throw std::runtime_error("erreur au début de l'enregistrement d'un command buffer!");
//         }

//         VkRenderPassBeginInfo renderPassInfo{};
//         renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//         renderPassInfo.renderPass = renderPass;
//         renderPassInfo.framebuffer = swapChainFramebuffers[i];

//         renderPassInfo.renderArea.offset = {0, 0};
//         renderPassInfo.renderArea.extent = swapChainExtent;

//         VkClearValue clearColor = {{{0.95f, 0.30f, 0.60f, 1.0f}}};
//         renderPassInfo.clearValueCount = 1;
//         renderPassInfo.pClearValues = &clearColor;

//         vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

//         vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->graphicsPipeline);

//         VkBuffer vertexBuffers[] = {vertexBuffer->buffer};
//         VkDeviceSize offsets[] = {0};
//         vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
//         vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer->buffer, 0, VK_INDEX_TYPE_UINT16);

//         vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipelineLayout, 0, 1, &descriptorSets[i], 0, nullptr);
//         vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);


//         vkCmdEndRenderPass(commandBuffers[i]);

//         if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
//             throw std::runtime_error("échec de l'enregistrement d'un command buffer!");
//         }
//     }
    

// }

void SwapChain::Destroy()
{
    std::cout << "[Destroying] SwapChain" << std::endl;


    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(*ctx.device, imageView, nullptr);
        // delete imageView;
    }
    vkDestroySwapchainKHR(*ctx.device, swapChain, nullptr);
}

SwapChain::~SwapChain(){
    Destroy();
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