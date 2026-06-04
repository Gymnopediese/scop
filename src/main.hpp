#pragma once

// STL
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <optional>
#include <set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <cstdint>
#include <limits>
// GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Vulkan
#include <vulkan/vulkan.h>
#include "math/Vertex/Vertex.hpp"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
};

// Utility
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;



struct VulkanContext
{
    VkInstance          *instance;
    VkDevice            *device;
    VkPhysicalDevice    *physicalDevice;
    VkQueue             *graphicsQueue;
    GLFWwindow          *window;
    VkSurfaceKHR        *surface;

    VulkanContext(){}
};



struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VulkanContext &ctx);
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VulkanContext &ctx);


#include "utils.hpp"
#include "image/PPM.hpp"


#include "vulkan/Window/Window.hpp"
#include "vulkan/Surface/Surface.hpp"
#include "shaders/Uniform/Uniform.hpp"
#include "vulkan/VImage/VImage.hpp"
#include "vulkan/ImageView/ImageView.hpp"
#include "vulkan/CommandBuffer/SingleUseCommandBuffer.hpp"
#include "vulkan/QueueFamilyIndices/QueueFamilyIndices.hpp"
#include "vulkan/ValidationLayers/ValidationLayers.hpp"
#include "vulkan/PhysicalDevice/PhysicalDevice.hpp"
#include "vulkan/Device/Device.hpp"
#include "vulkan/Instance/Instance.hpp"
#include "vulkan/Buffer/Buffer.hpp"


const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};


// Project-wide constants
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const int MAX_FRAMES_IN_FLIGHT = 2;


const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};


#ifdef NDEBUG
    constexpr bool enableValidationLayers = false;
#else
    constexpr bool enableValidationLayers = true;
#endif

