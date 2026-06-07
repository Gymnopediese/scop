# pragma once

#include "renderer/DescriptorManager/DescriptorManager.hpp"
# include "renderer/Renderer.hpp"
#include "vulkan/vulkan_core.h"


class FrameData;
// class DescriptorManager;
// class RenderPass;

class Renderer
{
    public:
        std::vector<FrameData *>        frames;
        std::vector<Buffer *>           objectsBuffers;
        std::vector<Buffer *>           cameraBuffers;
        std::vector<VkCommandBuffer>    commandBuffers;



        SwapChain                       swapchain;

        RenderPass                      *renderPass;
        RenderPass                      *postRenderPass;

        DescriptorManager               descriptors;
        DepthBuffer                     depthBuffer;
        VkCommandPool                   *commandPool;
        Scene3D                         scene;
        std::vector<VkFence>            imagesInFlight;
        size_t                          currentFrame = 0;

        std::vector<VkFramebuffer>      preProcessFramebuffers;
        std::vector<Image *>            preProcessImages;
        std::vector<Sampler *>          preProcessSamplers;
        Pipeline                        *postProcessPipeline;



        std::vector<VkFramebuffer>      swapChainFramebuffers;



        Renderer(VulkanContext &ctx);
        void drawFrame();
        ~Renderer();

        void createDescriptorPool();
        void createDescriptorSetLayout();
        void buildCommand(int imageIndex);


        void createFramebuffers();
    private:
        VulkanContext &ctx;
};