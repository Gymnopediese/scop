#include "Window.hpp"

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    w->framebufferResized = true;
}

Window::Window(VulkanContext &ctx) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    ctx.window = window;
}
    

Window::~Window()
{
    std::cout << "[Destroying] Window" << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();

}