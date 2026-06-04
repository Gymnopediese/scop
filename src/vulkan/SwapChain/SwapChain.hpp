#pragma once

#include "main.hpp"
// #include "../Pipeline/Pipeline.hpp"

#include "../Pipeline/Pipeline.hpp"



class Pipeline;

VkSurfaceFormatKHR  chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR    chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
VkExtent2D  chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, VulkanContext &ctx);

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VulkanContext &ctx);


class SwapChain {

    public:
        Buffer                          *vertexBuffer;
        Buffer                          *indexBuffer;
        std::vector<Buffer*>             uniformBuffers;

        VkDescriptorPool                descriptorPool;
        std::vector<VkDescriptorSet>    descriptorSets;

        VkDescriptorSetLayout           descriptorSetLayout;

        VkSwapchainKHR                  swapChain;
        VkExtent2D                      swapChainExtent;
        VkRenderPass                    renderPass;
        std::vector<VkImage>            swapChainImages;
        std::vector<VkCommandBuffer>    commandBuffers;
        SwapChain(VulkanContext &ctx);
        ~SwapChain();

        void Create();
        void Destroy();
        void ReCreate();

        void createSwapChain();
        void createImageViews();
        void createDescriptorSetLayout();
        void createCommandBuffers();
        void createCommandPool();
        void createFramebuffers();
        void createRenderPass();
        void createVertexBuffer();
        void createIndexBuffer();

        void createUniformBuffers();
        void createDescriptorPool();
        void createDescriptorSets();
    
    private:
        VulkanContext                   &ctx;
        VkFormat                        swapChainImageFormat;
        std::vector<ImageView*>         swapChainImageViews;
        std::vector<VkFramebuffer>      swapChainFramebuffers;
        VkCommandPool                   commandPool;
        Pipeline                        *pipeline;

};