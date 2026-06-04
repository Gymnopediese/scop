#pragma once

#include "main.hpp"
#include "../SwapChain/SwapChain.hpp"

class SwapChain;

class Pipeline
{
public:
    VkPipelineLayout                pipelineLayout;
    VkPipeline                      graphicsPipeline;

    Pipeline(VulkanContext &ctx, SwapChain &swapchain);
    ~Pipeline();

private:
    VkInstance                      instance;
    VulkanContext                   &ctx;
};
