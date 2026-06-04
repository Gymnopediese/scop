#pragma once

#include "main.hpp"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value();
    }
};


QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VulkanContext &ctx);
