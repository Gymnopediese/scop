
#include "Camera3D.hpp"
#include "shaders/Uniform/Camera.hpp"

Camera3D::Camera3D(VulkanContext &ctx) : ctx(ctx)
{
    // VkDeviceSize bufferSize = sizeof(CameraBufferObject);

    // uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    // for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    //     uniformBuffers[i] = new Buffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, ctx);
    // }
}

glm::mat4 Camera3D::getView()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    glm::vec3 direction = glm::normalize(front);

    return glm::lookAt(
        position,
        position + direction,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

void Camera3D::processMouse(float x, float y)
{
    yaw   += x * sensitivity;
    pitch -= y * sensitivity;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}


void Camera3D::update(uint32_t currentImage) {

    static float mouse_x = Inputs::mouse_x;
    static float mouse_y = Inputs::mouse_y;



    float dt = 1;
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0,1,0)));

    if (Inputs::is_action_pressed(GLFW_KEY_W))
        position += front * speed * dt;

    if (Inputs::is_action_pressed(GLFW_KEY_S))
        position -= front * speed * dt;

    if (Inputs::is_action_pressed(GLFW_KEY_A))
        position -= right * speed * dt;

    if (Inputs::is_action_pressed(GLFW_KEY_D))
        position += right * speed * dt;
    processMouse(Inputs::mouse_x - mouse_x,  Inputs::mouse_y- mouse_y);

    // float time = std::chrono::duration<float, std::chrono::seconds::period>(period).count();


    CameraBufferObject ubo{};
    // ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.f));
    ubo.model = glm::mat4(1.0f);
    // ubo.view = glm::lookAt(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    ubo.view = getView();
    // ubo.proj = glm::perspective(glm::radians(45.0f), ctx.swapChain->swapChainExtent.width / (float) ctx.swapChain->swapChainExtent.height, 0.1f, 100.0f);

    ubo.proj = glm::perspective(
        glm::radians(45.0f),
        ctx.swapChain->swapChainExtent.width /
        (float)ctx.swapChain->swapChainExtent.height,
        0.1f,
        100.0f
    );


    ubo.proj[1][1] *= -1;

    uniformBuffers[currentImage]->mapMemory(&ubo, sizeof(ubo));
    mouse_x = Inputs::mouse_x;
    mouse_y = Inputs::mouse_y;
}

Camera3D::~Camera3D()

{
    
}