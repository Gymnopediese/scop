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

    Scop(int argc, char ** argv){

        if (argc < 2) throw std::runtime_error("invalid args : scop <.obj file> (<.ppm texture>)");
        if (argc > 3) throw std::runtime_error("invalid args : scop <.obj file> (<.ppm texture>)");

        ctx.objPath = argv[1];
        if (argc == 3)
            ctx.textPath = argv[2];

        
        renderer = new Renderer(ctx);
    }

    void run() {
        mainLoop();
    }
        
    ~Scop() {
        delete renderer;
    }


private:

    VulkanContext                   ctx             = VulkanContext();

    Window                          window          = Window(ctx);
    Instance                        instance        = Instance(ctx);
    ValidationLayers                vlayers         = ValidationLayers(ctx);
    Surface                         surface         = Surface(ctx);
    PhysicalDevice                  physicalDevice  = PhysicalDevice(ctx);
    Device                          device          = Device(ctx);
    VkDebugUtilsMessengerEXT        callback;

    Renderer                        *renderer;


    void mainLoop() {


        glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSetKeyCallback(window.window, Inputs::keyCallback);
        glfwSetMouseButtonCallback(window.window, Inputs::mouseButtonCallback);
        glfwSetCursorPosCallback(window.window, Inputs::cursorPosCallback);
        glfwSetScrollCallback(window.window, Inputs::scrollCallback);

        double lastTime = glfwGetTime();

        while (!glfwWindowShouldClose(ctx.window->window)) {
            double currentTime = glfwGetTime();
            float delta = static_cast<float>(currentTime - lastTime);
            lastTime = currentTime;
        
            glfwPollEvents();

            if (glfwGetKey(ctx.window->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            if (glfwGetMouseButton(ctx.window->window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
                glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            renderer->drawFrame(delta);

            Inputs::clear();
            
        }
        vkDeviceWaitIdle(*ctx.device);
    }

};

int main(int argc, char ** argv) {



    try {
        std::cout << argc << std::endl;
        Scop scop(argc, argv);
        scop.run();
    } catch (const std::runtime_error& e) {
        std::cerr << "scop: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}