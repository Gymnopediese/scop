#pragma once

#include "main.hpp"

class Window {

    public:
        bool framebufferResized = false;
        
        Window(VulkanContext &ctx);
        ~Window();
    private:
        GLFWwindow*     window;

};