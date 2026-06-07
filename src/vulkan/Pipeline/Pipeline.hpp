#pragma once

#include "main.hpp"
#include "../SwapChain/SwapChain.hpp"

class SwapChain;

class Pipeline
{
public:
    VkPipelineLayout                pipelineLayout;
    VkPipeline                      graphicsPipeline;

    Pipeline(VulkanContext &ctx);
    ~Pipeline();

    static Pipeline *pipeline3D(VulkanContext &ctx);
    static Pipeline *pipelinePostProcess(VulkanContext &ctx);

private:
    VkInstance                      instance;
    VulkanContext                   &ctx;
};
