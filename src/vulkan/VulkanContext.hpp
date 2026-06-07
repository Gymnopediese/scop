
#pragma once

#include "main.hpp"
#include "vulkan/vulkan_core.h"


class Window;
class SwapChain;

struct VulkanContext
{
    VkInstance              *instance;
    VkDevice                *device;
    VkPhysicalDevice        *physicalDevice;
    VkQueue                 *graphicsQueue;
    VkQueue                 *presentQueue;
    Window                  *window;
    VkSurfaceKHR            *surface;
    VkCommandPool           *commandPool;
    SwapChain               *swapChain;
    VkRenderPass            *processPass;
    VkRenderPass            *postProcessPass;
    VkDescriptorSetLayout   *descriptorSetLayout;
    VkDescriptorSetLayout   *postProcessingLayout;

    VulkanContext(){}
};

#include "vulkan/Window/Window.hpp"
#include "SwapChain/SwapChain.hpp"
#include "Pipeline/Pipeline.hpp"
#include "CommandBuffer/SingleUseCommandBuffer.hpp"
#include "QueueFamilyIndices/QueueFamilyIndices.hpp"
#include "Inputs/Inputs.hpp"