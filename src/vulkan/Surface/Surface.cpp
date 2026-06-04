
#include "Surface.hpp"

Surface::Surface(VulkanContext &ctx) : ctx(ctx) {
    if (glfwCreateWindowSurface(*ctx.instance, ctx.window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("échec de la création de la window surface!");
    }
    ctx.surface = &surface;
}

Surface::~Surface() {
    vkDestroySurfaceKHR(*ctx.instance, surface, nullptr);
}