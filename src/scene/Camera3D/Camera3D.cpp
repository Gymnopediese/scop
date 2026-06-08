
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

mat4 Camera3D::getView()
{
    vec3 front;
    front.x = cos(radians(yaw)) * cos(radians(pitch));
    front.y = sin(radians(pitch));
    front.z = sin(radians(yaw)) * cos(radians(pitch));

    vec3 direction = front.normalize();

    return mat4::lookAt(
        position,
        position + direction,
        vec3(0.0f, 1.0f, 0.0f)
    );
}

void Camera3D::processMouse(float x, float y, float delta)
{
    yaw   += x * sensitivity * delta;
    pitch -= y * sensitivity * delta;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

void dumpHex(const mat4& obj)
{
    const unsigned char* p =
        reinterpret_cast<const unsigned char*>(&obj);

    for (size_t i = 0; i < sizeof(obj); i++) {
        printf("%02X ", p[i]);

        if ((i + 1) % 16 == 0)
            printf("\n");
    }

    printf("\n");
}


void Camera3D::update(uint32_t currentImage, float delta) {

    static float mouse_x = MAXFLOAT;
    static float mouse_y = MAXFLOAT;

    if (mouse_x == MAXFLOAT)
    {
        mouse_x = Inputs::mouse_x;
        mouse_y = Inputs::mouse_y;
    }


    vec3 front;
    front.x = cos(radians(yaw)) * cos(radians(pitch));
    front.y = sin(radians(pitch));
    front.z = sin(radians(yaw)) * cos(radians(pitch));
    front = front.normalize();

    vec3 right = front.cross(vec3(0,1,0)).normalize();

    if (Inputs::is_action_pressed(GLFW_KEY_W))
        position += front * speed * delta;

    if (Inputs::is_action_pressed(GLFW_KEY_S))
        position -= front * speed * delta;

    if (Inputs::is_action_pressed(GLFW_KEY_A))
        position -= right * speed * delta;

    if (Inputs::is_action_pressed(GLFW_KEY_D))
        position += right * speed * delta;
    processMouse(Inputs::mouse_x - mouse_x,  Inputs::mouse_y- mouse_y, delta);

    // float time = std::chrono::duration<float, std::chrono::seconds::period>(period).count();


    CameraBufferObject ubo{};
    // ubo.model = rotate(mat4(1.0f), time * radians(90.0f), vec3(0.0f, 1.0f, 0.f));
    ubo.model = mat4::identity();
    // ubo.view = lookAt(vec3(5.0f, 5.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));

    ubo.view = getView();
    // ubo.view = mat4::lookAt(vec3(10.0f, 10.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));
    // ubo.proj = perspective(radians(45.0f), ctx.swapChain->swapChainExtent.width / (float) ctx.swapChain->swapChainExtent.height, 0.1f, 100.0f);

    ubo.proj = mat4::perspective(
        radians(45.0f),
        ctx.swapChain->swapChainExtent.width /
        (float)ctx.swapChain->swapChainExtent.height,
        0.1f, 
        100.0f
    );

    uniformBuffers[currentImage]->mapMemory(&ubo, sizeof(ubo));
    mouse_x = Inputs::mouse_x;
    mouse_y = Inputs::mouse_y;
}

Camera3D::~Camera3D()

{
    
}