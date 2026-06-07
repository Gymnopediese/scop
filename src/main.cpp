#include "main.hpp"
#include "renderer/Renderer/Renderer.hpp"
#include "scene/Scene3D/Scene3D.hpp"



#include "vulkan/Device/Device.hpp"
#include "vulkan/Instance/Instance.hpp"
#include "vulkan/PhysicalDevice/PhysicalDevice.hpp"
#include "vulkan/Surface/Surface.hpp"
#include "vulkan/SwapChain/SwapChain.hpp"
#include "vulkan/ValidationLayers/ValidationLayers.hpp"
#include "vulkan/Window/Window.hpp"



class Scop {


public:

    Scop(){}

    void run() {
        mainLoop();
    }
        
    ~Scop() {}


private:

    VulkanContext                   ctx             = VulkanContext();

    Window                          window          = Window(ctx);
    Instance                        instance        = Instance(ctx);
    ValidationLayers                vlayers         = ValidationLayers(ctx);
    Surface                         surface         = Surface(ctx);
    PhysicalDevice                  physicalDevice  = PhysicalDevice(ctx);
    Device                          device          = Device(ctx);
    VkDebugUtilsMessengerEXT        callback;

    Renderer                        renderer = Renderer(ctx);


    void mainLoop() {
        std::cout << "ah bah au moin est la ! " << std::endl;

        glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSetKeyCallback(window.window, Inputs::keyCallback);
        glfwSetMouseButtonCallback(window.window, Inputs::mouseButtonCallback);
        glfwSetCursorPosCallback(window.window, Inputs::cursorPosCallback);
        glfwSetScrollCallback(window.window, Inputs::scrollCallback);


        while (!glfwWindowShouldClose(ctx.window->window)) {
            glfwPollEvents();

            // if (glfwGetKey(ctx.window->window, GLFW_KEY_W)) {
            //     Inputs::events.push_back(InputEvent(InputEvent::Key, GLFW_KEY_W, glfwGetKey(ctx.window->window, GLFW_KEY_W)));
            // }
            // if (glfwGetKey(ctx.window->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            //     glfwSetWindowShouldClose(ctx.window->window, true);
            // }
            renderer.drawFrame();
            Inputs::clear();
            
        }
        vkDeviceWaitIdle(*ctx.device);
    }

};

int main() {
    Scop scop;

    try {
        scop.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}