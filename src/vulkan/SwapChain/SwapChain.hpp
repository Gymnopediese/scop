#pragma once

#include "vulkan/VulkanContext.hpp"

VkSurfaceFormatKHR      chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR        chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
VkExtent2D              chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, VulkanContext &ctx);
SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VulkanContext &ctx);


class SwapChain {

    public:


        VkSwapchainKHR              swapChain;   
        VkFormat                    swapChainImageFormat;
        VkExtent2D                  swapChainExtent;

        std::vector<VkImage>        swapChainImages;
        std::vector<VkImageView>    swapChainImageViews;

        SwapChain(VulkanContext &ctx);
        ~SwapChain();

        void Create();
        void Destroy();
        void ReCreate();

        void createSwapChain();
        void createImageViews();

    
    private:
        VulkanContext                   &ctx;

};