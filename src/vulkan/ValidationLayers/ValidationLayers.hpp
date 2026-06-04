#pragma once

#include "main.hpp"

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);
bool checkValidationLayerSupport();
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);


class ValidationLayers
{
    public:

        ValidationLayers(VulkanContext &ctx);
        ~ValidationLayers();


    private:
        VulkanContext                   &ctx;
        VkDebugUtilsMessengerEXT        callback;

};
