#pragma once

#include "main.hpp"

struct VulkanContext;

class Window {

    public:
        bool framebufferResized = false;
        
        Window(VulkanContext &ctx);
        ~Window();
        GLFWwindow*     window;

};