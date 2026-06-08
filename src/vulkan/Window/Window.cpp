#include "Window.hpp"

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    w->framebufferResized = true;
}

Window::Window(VulkanContext &ctx) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(WIDTH, HEIGHT, "scop", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    ctx.window = this;

    std::cout << "[Window] DONE" << std::endl;
}
    

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "[Destroying] Window" << std::endl;

}