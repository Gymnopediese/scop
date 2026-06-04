
#include "PhysicalDevice.hpp"

bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}


bool isDeviceSuitable(VkPhysicalDevice device, VulkanContext &ctx) {
    QueueFamilyIndices indices = findQueueFamilies(device, ctx);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, ctx);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

PhysicalDevice::PhysicalDevice(VulkanContext &ctx) :ctx(ctx) {

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(*ctx.instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("aucune carte graphique ne supporte Vulkan!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(*ctx.instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device, ctx)) {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("aucun GPU ne peut exécuter ce programme!");
    }
    ctx.physicalDevice = &physicalDevice;

}

PhysicalDevice::~PhysicalDevice(){
    
    std::cout << "[Destroying] PhysicalDevice" << std::endl;
    vkDestroyDevice(*ctx.device, nullptr);
}