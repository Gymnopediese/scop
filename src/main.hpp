#pragma once

#include "lib.hpp"
// Utility
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

#define MAX_TEXTURES 1000
#define MAX_OBJECTS  1000


struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct VulkanContext;

bool checkValidationLayerSupport();
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
#include "vulkan/VulkanContext.hpp"
SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VulkanContext &ctx);
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VulkanContext &ctx);


#include "math/Vertex/Vertex.hpp"
#include "utils.hpp"
#include "loader/PPM.hpp"
#include "shaders/Uniform/Camera.hpp"
#include "shaders/Uniform/Object.hpp"

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

