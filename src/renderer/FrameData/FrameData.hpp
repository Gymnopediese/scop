
#pragma once

#include "renderer/Renderer.hpp"

class FrameData  {
    public:

        VkFence         inFlight;
        VkSemaphore     imageAvailable;
        VkSemaphore     renderFinished;


        Buffer          *cameraBuffer;

        
        
        FrameData(VulkanContext &ctx);
        ~FrameData();



    private:
        VulkanContext &ctx;

};