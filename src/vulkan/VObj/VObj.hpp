#pragma once

#include "main.hpp"

class VulkanObject {
protected:
    VulkanContext& ctx;
    
    explicit VulkanObject(VulkanContext& ctx) : ctx(ctx) {}
    
    // Pas de copie — une référence ne doit pas être réassignée
    VulkanObject(const VulkanObject&)            = delete;
    VulkanObject& operator=(const VulkanObject&) = delete;
    
    // Move ok
    VulkanObject(VulkanObject&&)                 = default;
};