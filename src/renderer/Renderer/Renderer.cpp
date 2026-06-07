

#include "Renderer.hpp"
#include "assets/Texture/Texture.hpp"
#include "vulkan/VulkanContext.hpp"
#include "vulkan/vulkan_core.h"
#include <vector>

VkCommandPool *createCommandPool(VulkanContext &ctx) {
    VkCommandPool *commandPool = new VkCommandPool();
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(*ctx.physicalDevice, ctx);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optionel
    

    if (vkCreateCommandPool(*ctx.device, &poolInfo, nullptr, commandPool) != VK_SUCCESS) {
        throw std::runtime_error("échec de la création d'une command pool!");
    }

    ctx.commandPool = commandPool;
    std::cout << "[COMMANDPOOL] DONE" << std::endl;
    return commandPool;
}




Renderer::Renderer(VulkanContext &ctx) :
    ctx(ctx),
    renderPass(RenderPass::preProcessPass(ctx)),
    postRenderPass(RenderPass::postProcessPass(ctx)),
    swapchain(SwapChain(ctx)),
    commandPool(createCommandPool(ctx)),
    scene(Scene3D(ctx)),
    descriptors(DescriptorManager(ctx)),
    depthBuffer(DepthBuffer(ctx))
{
    // exit(0);

    postProcessPipeline = Pipeline::pipelinePostProcess(ctx);

    imagesInFlight.resize(swapchain.swapChainImages.size(), VK_NULL_HANDLE);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        frames.push_back(new FrameData(ctx));
        cameraBuffers.push_back(frames[i]->cameraBuffer);
    }


    preProcessImages.resize(swapchain.swapChainImages.size());
    preProcessSamplers.resize(swapchain.swapChainImages.size());
    for (size_t i = 0; i < swapchain.swapChainImages.size(); i++) {
        preProcessImages[i] = (new Image(swapchain.swapChainExtent.width, swapchain.swapChainExtent.height, ctx, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT , swapchain.swapChainImageFormat));
        preProcessSamplers[i] = (new Sampler(ctx));
    }


    std::cout << "[FRAMES] DONE" << std::endl;

    // std::cout << "[BINDING] BICTCH 0" << std::endl;
    // descriptors.bindCameraBuffers(cameraBuffers);
    // std::cout << "[BINDING] BICTCH 1" << std::endl;
    // descriptors.bindObjectBuffers(objectsBuffers);
    // std::cout << "[BINDING] BICTCH 2" << std::endl;
    // descriptors.bindTextureData(Texture::all_textures);
    // std::cout << "[BINDING] BICTCH" << std::endl;
    

    scene.camera.uniformBuffers = cameraBuffers;

    createFramebuffers();    

    descriptors.setObjectDescriptor(*scene.objects[0], scene.camera);
    descriptors.createPostPorcessSets(preProcessImages, preProcessSamplers);
    std::cout << "[DESCRIPTORSET] DONE" << std::endl;

    commandBuffers.resize(swapchain.swapChainImages.size());
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = *ctx.commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) swapchain.swapChainImages.size();

    if (vkAllocateCommandBuffers(*ctx.device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("échec de l'allocation de command buffers!");
    }
    std::cout << "[RENDERER] DONE" << std::endl;


}


void Renderer::drawFrame() {
    
    // std::cout << "[FRAMERENDERING] START" << std::endl;

    vkWaitForFences(*ctx.device, 1, &frames[currentFrame]->inFlight, VK_TRUE, UINT64_MAX);


    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(*ctx.device, swapchain.swapChain, UINT64_MAX, frames[currentFrame]->imageAvailable, VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        swapchain.ReCreate();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("échec de la présentation d'une image à la swap chain!");
    }

    // std::cout << "[FRAMERENDERING] IMAGE AQUIRED" << std::endl;

    // Vérifier si une frame précédente est en train d'utiliser cette image (il y a une fence à attendre)
    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(*ctx.device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    // Marque l'image comme étant à nouveau utilisée par cette frame
    imagesInFlight[imageIndex] = frames[currentFrame]->inFlight;
    // std::cout << "[FRAMERENDERING] FENCE 1 PASSED" << std::endl;


    scene.update(currentFrame);
    // std::cout << "[FRAMERENDERING] SCENE UPDATED" << std::endl;



    buildCommand(imageIndex);
    // std::cout << "[FRAMERENDERING] COMMAND BUILT" << std::endl;


    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {frames[currentFrame]->imageAvailable};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = {frames[currentFrame]->renderFinished};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(*ctx.device, 1, &frames[currentFrame]->inFlight);

    if (vkQueueSubmit(*ctx.graphicsQueue, 1, &submitInfo, frames[currentFrame]->inFlight) != VK_SUCCESS) {
        throw std::runtime_error("échec de l'envoi d'un command buffer!");
    }
    vkQueueWaitIdle(*ctx.graphicsQueue);

    // std::cout << "[FRAMERENDERING] SUBMITED SHIT" << std::endl;


    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapchain.swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    presentInfo.pResults = nullptr; // Optionnel

    result = vkQueuePresentKHR(*ctx.presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || ctx.window->framebufferResized) 
    {
        ctx.window->framebufferResized = false;
        swapchain.ReCreate();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("échec de la présentation d'une image!");
    }


    // std::cout << "[FRAMERENDERING] DONE" << std::endl;

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::buildCommand(int imageIndex)
{

    // std::cout << "[COMMANDBUILD] START COMMAND BUILD" << std::endl;
    vkResetCommandBuffer(commandBuffers[imageIndex], 0);
    // std::cout << "[COMMANDBUILD] RESET" << std::endl;
    
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optionnel
    beginInfo.pInheritanceInfo = nullptr; // Optionel

    if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("erreur au début de l'enregistrement d'un command buffer!");
    }
    // std::cout << "[COMMANDBUILD] BEGIN" << std::endl;

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass->renderPass;
    renderPassInfo.framebuffer = preProcessFramebuffers[imageIndex];

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapchain.swapChainExtent;


    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.95f, 0.30f, 0.60f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();


    vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // std::cout << "[COMMANDBUILD] RENDERPASS START" << std::endl;

    for (Object3D *object : scene.objects)
    {

        vkCmdBindPipeline(commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, object->material->pipeline->graphicsPipeline);

        // std::cout << "[COMMANDBUILD] BIND PIPELINE" << std::endl;
        
        VkBuffer vertexBuffers[] = {object->mesh->vertexBuffer->buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffers[imageIndex], 0, 1, vertexBuffers, offsets);
        // std::cout << "[COMMANDBUILD] BIND VERTICES" << std::endl;

        vkCmdBindIndexBuffer(commandBuffers[imageIndex], object->mesh->indexBuffer->buffer, 0, VK_INDEX_TYPE_UINT32);
        
        // std::cout << "[COMMANDBUILD] BIND INDICES" << std::endl;

        vkCmdBindDescriptorSets(commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, object->material->pipeline->pipelineLayout, 0, 1, &object->descriptorSets[currentFrame], 0, nullptr);
        vkCmdDrawIndexed(commandBuffers[imageIndex], static_cast<uint32_t>(object->mesh->indices.size()), 1, 0, 0, 0);

        // std::cout << "[COMMANDBUILD] DRAW INDEX" << std::endl;
    }
    // vkCmdDraw(commandBuffers[imageIndex], 3, 1, 0, 0);
    
    // std::cout << "[COMMANDBUILD] OBJECT" << std::endl;
    vkCmdEndRenderPass(commandBuffers[imageIndex]);



    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = postRenderPass->renderPass;
    renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapchain.swapChainExtent;

    clearValues[0].color = {{0.95f, 0.30f, 0.40f, 1.0f}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, postProcessPipeline->graphicsPipeline);

    vkCmdBindDescriptorSets(commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, postProcessPipeline->pipelineLayout, 0, 1, &descriptors.postProcessingSets[imageIndex], 0, nullptr);
    
    vkCmdDraw(commandBuffers[imageIndex], 4, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffers[imageIndex]);
    // std::cout << "[COMMANDBUILD] RENDERPASS END" << std::endl;

    if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
        throw std::runtime_error("échec de l'enregistrement d'un command buffer!");
    }
    // std::cout << "[COMMANDBUILD] DONE" << std::endl;
}

void Renderer::createFramebuffers() {
    preProcessFramebuffers.resize(swapchain.swapChainImageViews.size());
    swapChainFramebuffers.resize(swapchain.swapChainImageViews.size());
    
    for (size_t i = 0; i < swapchain.swapChainImageViews.size(); i++) {

        std::cout << preProcessImages[i]->textureImageView->imageView << std::endl;
        std::array<VkImageView, 2> attachments = {
            preProcessImages[i]->textureImageView->imageView,
            depthBuffer.depthImageView->imageView,
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass->renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapchain.swapChainExtent.width;
        framebufferInfo.height = swapchain.swapChainExtent.height;
        framebufferInfo.layers = 1;
        
        
        if (vkCreateFramebuffer(*ctx.device, &framebufferInfo, nullptr, &preProcessFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("échec de la création d'un framebuffer!");
        }
        std::cout << "[FRAMEBUFFER] 1 created" << std::endl;

        attachments = {
            swapchain.swapChainImageViews[i],
            // depthBuffer.depthImageView->imageView,
        };
        framebufferInfo.renderPass = postRenderPass->renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments.data();

        if (vkCreateFramebuffer(*ctx.device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("échec de la création d'un framebuffer!");
        }
    }

}



Renderer::~Renderer()
{
    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(*ctx.device, framebuffer, nullptr);
    }
}

