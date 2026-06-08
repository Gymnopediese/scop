

#include "FrameData.hpp"
#include "shaders/Uniform/Camera.hpp"
#include "shaders/Uniform/Object.hpp"


FrameData::FrameData(VulkanContext &ctx) : ctx(ctx)
{





    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    

    if (vkCreateSemaphore(*ctx.device, &semaphoreInfo, nullptr, &imageAvailable) != VK_SUCCESS ||
        vkCreateSemaphore(*ctx.device, &semaphoreInfo, nullptr, &renderFinished) != VK_SUCCESS ||
        vkCreateFence(*ctx.device, &fenceInfo, nullptr, &inFlight) != VK_SUCCESS) {

        throw std::runtime_error("échec de la création des objets de synchronisation pour une frame!");
    }

    VkDeviceSize bufferSize = sizeof(CameraBufferObject);
    cameraBuffer = new Buffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, ctx);


}

FrameData::~FrameData()
{
    if (imageAvailable == VK_NULL_HANDLE)
        return;
    vkDestroySemaphore(*ctx.device, imageAvailable, nullptr);
    vkDestroySemaphore(*ctx.device, renderFinished, nullptr);
    vkDestroyFence(*ctx.device, inFlight, nullptr);
    delete cameraBuffer;
}