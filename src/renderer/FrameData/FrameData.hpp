
#pragma once

#include "renderer/Renderer.hpp"

class FrameData  {
    public:

        VkFence         inFlight;
        VkSemaphore     imageAvailable;
        VkSemaphore     renderFinished;


        Buffer          *cameraBuffer;
        Buffer          *objectTransformBuffer;

        
        
        FrameData(VulkanContext &ctx);
        ~FrameData();



    private:
        VulkanContext &ctx;

};