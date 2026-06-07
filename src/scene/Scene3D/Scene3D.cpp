
#include "Scene3D.hpp"
#include "assets/Material/Material.hpp"
#include "assets/Texture/Texture.hpp"
#include "main.hpp"
#include "resources/Buffer/Buffer.hpp"
#include <iostream>

void populateMesh(Mesh &mesh, VulkanContext &ctx) {
    VkDeviceSize bufferSize = sizeof(mesh.vertices[0]) * mesh.vertices.size();

    Buffer stagingBuffer = Buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, ctx);
    stagingBuffer.mapMemory(mesh.vertices.data(), bufferSize);
    mesh.vertexBuffer = new Buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, ctx);
    mesh.vertexBuffer->copyBuffer(stagingBuffer, bufferSize, *ctx.commandPool);

    bufferSize = sizeof(mesh.indices[0]) * mesh.indices.size();
    Buffer stagingBuffer2 = Buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, ctx);
    stagingBuffer2.mapMemory(mesh.indices.data(), bufferSize);
    mesh.indexBuffer = new Buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, ctx);
    mesh.indexBuffer->copyBuffer(stagingBuffer2, bufferSize, *ctx.commandPool);

}

// void SwapChain::createIndexBuffer() {
//     VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

//     Buffer stagingBuffer = Buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, ctx);

//     void* data;
//     vkMapMemory(*ctx.device, stagingBuffer.bufferMemory, 0, bufferSize, 0, &data);
//     memcpy(data, indices.data(), (size_t) bufferSize);
//     vkUnmapMemory(*ctx.device, stagingBuffer.bufferMemory);

//     indexBuffer = new Buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, ctx);

//     indexBuffer->copyBuffer(stagingBuffer, bufferSize, commandPool);
// }

void Scene3D::testScene()
{
    objects.resize(7);
    objects[0] = new Object3D();
    objects[1] = new Object3D();
    objects[2] = Object3D::loadOBJ(ctx, "assets/42.obj");
    objects[3] = Object3D::loadOBJ(ctx, "assets/viking_room.obj", "assets/viking_room.ppm");
    objects[4] = Object3D::loadOBJ(ctx, "assets/homme.obj");
    objects[6] = Object3D::loadOBJ(ctx, "assets/teapot.obj");


    objects[5] = Object3D::loadOBJ(ctx, "assets/Eyeball/eyeball.obj", "assets/Eyeball/eyeball.ppm");



    objects[0]->mesh = new Mesh();
    objects[0]->mesh->vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1, 1, 1}, {1.f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1, 1, 1}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1, 1, 1}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, { 1, 1, 1}, {1.0f, 1.0f}},
    };
    objects[0]->mesh->indices = {
        0, 1, 2, 2, 3, 0,
    };

    objects[1]->mesh = new Mesh();
    objects[1]->mesh->vertices = {
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1, 1, 1}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1, 1, 1}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1, 1, 1}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, { 1, 1, 1}, {1.0f, 1.0f}}
    };
    objects[1]->mesh->indices = {
        0, 1, 2, 2, 3, 0,
    };

    glm::mat4 transform = {
        {1, 0, 0, 0},
        {0,1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    };

    transform[3] = glm::vec4(0.0f, 0.0f, 0, 1.0f);



    objects[0]->transform.model = transform;
    objects[1]->transform.model = transform;
    objects[2]->transform.model = transform;
    objects[2]->transform.use_texture = 0;
    objects[3]->transform.model = transform;
    objects[4]->transform.model = transform;
    objects[5]->transform.model = transform;
    objects[6]->transform.model = transform;


    objects[3]->transform.model[3] = glm::vec4(5, 5, 0., 1.0f);

    objects[4]->transform.model[3] = glm::vec4(5, 0, 5., 1.0f);
    objects[5]->transform.model[3] = glm::vec4(0, 0, 10., 1.0f);
    objects[6]->transform.model[3] = glm::vec4(5, 0, 0., 1.0f);
    objects[6]->transform.use_texture = 0;

    objects[3]->transform.model = glm::rotate(objects[3]->transform.model, glm::radians(-90.0f),glm::vec3(1.0f, 0.0f, 2.f));

    objects[2]->color({1, 0.3, 0.7});
    objects[6]->color({0.8, 0.3, 1});

    populateMesh(*objects[0]->mesh, ctx);
    populateMesh(*objects[1]->mesh, ctx);
    populateMesh(*objects[2]->mesh, ctx);
    populateMesh(*objects[3]->mesh, ctx);
    populateMesh(*objects[4]->mesh, ctx);
    populateMesh(*objects[5]->mesh, ctx);
    populateMesh(*objects[6]->mesh, ctx);


    objects[0]->material = new Material(ctx);
    objects[0]->material->pipeline = Pipeline::pipeline3D(ctx);
    objects[1]->material = objects[0]->material;
    objects[2]->material = objects[0]->material;
    objects[3]->material = objects[0]->material;
    objects[4]->material = objects[0]->material;
    objects[5]->material = objects[0]->material;
    objects[6]->material = objects[0]->material;

    std::cout << "[SCENE3D] MAKING MATS" << std::endl;

    objects[0]->texture = Texture::load_ppm("assets/pupilas.ppm", ctx);
    objects[1]->texture = Texture::load_ppm("assets/futeboll.ppm", ctx);
    objects[2]->texture = objects[0]->texture;
    objects[4]->texture = objects[0]->texture;
    objects[6]->texture = objects[0]->texture;


    for (Object3D *object : objects)
    {
        
        object->transformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        for (int i = 0 ; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            VkDeviceSize bufferSize = sizeof(Transform3D);
            object->transformBuffers[i] = new Buffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, ctx);
            object->transformBuffers[i]->mapMemory(&object->transform, bufferSize);
        }
        
        std::cout << "[SCENE3D] OBJECT BUFFER DONE" << std::endl;
    }
    std::cout << "[SCENE3D] DONE" << std::endl;




}

Scene3D::~Scene3D()
{
    delete objects[0];
    delete objects[1];
}