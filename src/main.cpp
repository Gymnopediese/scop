#include "main.hpp"
#include "vulkan/SwapChain/SwapChain.hpp"





// struct SwapChainSupportDetails {
//     VkSurfaceCapabilitiesKHR capabilities;
//     std::vector<VkSurfaceFormatKHR> formats;
//     std::vector<VkPresentModeKHR> presentModes;
// };


// VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) {
//     auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
//     if (func != nullptr) {
//         return func(instance, pCreateInfo, pAllocator, pCallback);
//     } else {
//         return VK_ERROR_EXTENSION_NOT_PRESENT;
//     }
// }

class HelloTriangleApplication {


public:
    void run() {
        createSyncObjects();
        std::cout << "Project start running !" << std::endl;
        mainLoop();
    }
        
    ~HelloTriangleApplication() {
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(*ctx.device, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(*ctx.device, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(*ctx.device, inFlightFences[i], nullptr);
        }
    }



private:
    VulkanContext                   ctx             = VulkanContext();
    Window                          window          = Window(ctx);
    Instance                        instance        = Instance(ctx);
    ValidationLayers                vlayers         = ValidationLayers(ctx);
    Surface                         surface         = Surface(ctx);
    PhysicalDevice                  physicalDevice  = PhysicalDevice(ctx);
    Device                          device          = Device(ctx);
    SwapChain                       swapchain       = SwapChain(ctx);

    VkDebugUtilsMessengerEXT        callback;

    std::vector<VkSemaphore>        imageAvailableSemaphores;
    std::vector<VkSemaphore>        renderFinishedSemaphores;
    std::vector<VkFence>            inFlightFences;
    std::vector<VkFence>            imagesInFlight;
    size_t                          currentFrame = 0;


    void createSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        imagesInFlight.resize(swapchain.swapChainImages.size(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(*ctx.device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(*ctx.device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(*ctx.device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {

                throw std::runtime_error("échec de la création des objets de synchronisation pour une frame!");
            }
        }
    }


    void mainLoop() {
        while (!glfwWindowShouldClose(ctx.window)) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(*ctx.device);
    }

    void drawFrame() {
        vkWaitForFences(*ctx.device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);


        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(*ctx.device, swapchain.swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            swapchain.ReCreate();
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("échec de la présentation d'une image à la swap chain!");
        }


        // Vérifier si une frame précédente est en train d'utiliser cette image (il y a une fence à attendre)
        if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(*ctx.device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }
        // Marque l'image comme étant à nouveau utilisée par cette frame
        imagesInFlight[imageIndex] = inFlightFences[currentFrame];


        updateUniformBuffer(imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &swapchain.commandBuffers[imageIndex];

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(*ctx.device, 1, &inFlightFences[currentFrame]);

        if (vkQueueSubmit(device.graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("échec de l'envoi d'un command buffer!");
        }


        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapchain.swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        presentInfo.pResults = nullptr; // Optionnel

        result = vkQueuePresentKHR(device.presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.framebufferResized) 
        {
            window.framebufferResized = false;
            swapchain.ReCreate();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("échec de la présentation d'une image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }


    void updateUniformBuffer(uint32_t currentImage) {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();


        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        ubo.proj = glm::perspective(glm::radians(45.0f), swapchain.swapChainExtent.width / (float) swapchain.swapChainExtent.height, 0.1f, 10.0f);

        ubo.proj[1][1] *= -1;

        swapchain.uniformBuffers[currentImage]->mapMemory(&ubo, sizeof(ubo));
    }
    

    // void extention() {
        
    //     uint32_t extensionCount = 0;
    //     vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);


    //     std::vector<VkExtensionProperties> extensions(extensionCount);
    //     vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    //     std::cout << "Extensions disponibles :\n";

    //     for (const auto& extension : extensions) {
    //         std::cout << '\t' << extension.extensionName << '\n';
    //     }

    // }



  


};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}