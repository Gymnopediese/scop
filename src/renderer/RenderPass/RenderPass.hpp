


#pragma once

#include "renderer/Renderer.hpp"

class RenderPass {


    public:

        VkRenderPass renderPass;
        RenderPass(VulkanContext &ctx);
        ~RenderPass();

        static RenderPass *postProcessPass(VulkanContext & ctx);
        static RenderPass *preProcessPass(VulkanContext & ctx);


    private:
        VulkanContext &ctx;

};