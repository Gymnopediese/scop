
#include "DescriptorManager.hpp"
#include "main.hpp"
#include "vulkan/vulkan_core.h"

DescriptorManager::DescriptorManager(VulkanContext &ctx) : ctx(ctx)
{
    std::cout << "[DESCRIPTORMANAGE] c'est mon tour !" << std::endl;
    createPostProcessLayout();
    createDescriptorSetLayout();
    std::cout << "[DESCRIPTORMANAGE] LAYOUTE DONE" << std::endl;
    createDescriptorPool();
    // std::cout << "[DESCRIPTORMANAGE] DESCRIPTOR POOL DONE" << std::endl;
    // createCameraData();
    std::cout << "[DESCRIPTORMANAGE] CAMERA SET DONE" << std::endl;
    // createObjectsData();
    // std::cout << "[DESCRIPTORMANAGE] OBJECT SET DONE" << std::endl;
    // createTexturesData();
    // std::cout << "[DESCRIPTORMANAGE] TEXTURE SET DONE" << std::endl;

    ctx.descriptorSetLayout = &descriptorSetLayout;
    ctx.postProcessingLayout = &postProcessingLayout;
    std::cout << "[DESCRIPTORMANAGE] DONE" << std::endl;
}


void DescriptorManager::createDescriptorPool() {

    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = MAX_OBJECTS * MAX_FRAMES_IN_FLIGHT * 2;

    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = MAX_OBJECTS * MAX_FRAMES_IN_FLIGHT;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();

    poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT * MAX_OBJECTS * 3;
    
    if (vkCreateDescriptorPool(*ctx.device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("echec de la creation de la pool de descripteurs!");
    }


}

void DescriptorManager::createDescriptorSetLayout()
{
    // Binding 0 → Camera (UBO or SSBO)
    VkDescriptorSetLayoutBinding camera{};
    camera.binding = 0;
    camera.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    camera.descriptorCount = 1;
    camera.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

    // Binding 1 → Object buffer (ALL transforms in one SSBO)
    VkDescriptorSetLayoutBinding objects{};
    objects.binding = 1;
    objects.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    objects.descriptorCount = 1;
    objects.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    // Binding 2 → Bindless textures
    VkDescriptorSetLayoutBinding textures{};
    textures.binding = 2;
    textures.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

    textures.descriptorCount = 1; // IMPORTANT
    textures.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorBindingFlags flags[] = {0, 0, 0};

    VkDescriptorSetLayoutBindingFlagsCreateInfo flagsInfo{};
    flagsInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
    flagsInfo.bindingCount = 3;
    flagsInfo.pBindingFlags = flags;

    std::array<VkDescriptorSetLayoutBinding, 3> bindings = {
        camera,
        objects,
        textures
    };

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = (uint32_t)bindings.size();
    layoutInfo.pBindings = bindings.data();
    layoutInfo.pNext = &flagsInfo;


    if (vkCreateDescriptorSetLayout(*ctx.device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("echec de la creation d'un set de descripteurs!");
    }
}



void DescriptorManager::createPostProcessLayout()
{
    // Binding 2 → Bindless textures
    VkDescriptorSetLayoutBinding textures{};
    textures.binding = 0;
    textures.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

    textures.descriptorCount = 1; // IMPORTANT
    textures.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorBindingFlags flags[] = {0};

    VkDescriptorSetLayoutBindingFlagsCreateInfo flagsInfo{};
    flagsInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
    flagsInfo.bindingCount = 1;
    flagsInfo.pBindingFlags = flags;

    std::array<VkDescriptorSetLayoutBinding, 1> bindings = {
        textures
    };

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = (uint32_t)bindings.size();
    layoutInfo.pBindings = bindings.data();
    layoutInfo.pNext = &flagsInfo;

    if (vkCreateDescriptorSetLayout(*ctx.device, &layoutInfo, nullptr, &postProcessingLayout) != VK_SUCCESS) {
        throw std::runtime_error("echec de la creation d'un set de descripteurs!");
    }
}

// void DescriptorManager::createTexturesData()
// {

//     if (vkAllocateDescriptorSets(*ctx.device, &allocInfo, &texturesSet) != VK_SUCCESS) {
//         throw std::runtime_error("failed to allocate bindless descriptor set");
//     }
// }

void DescriptorManager::createPostPorcessSets(std::vector<Image *> &images, std::vector<Sampler *> &samplers)
{
    postProcessingSets.resize(ctx.swapChain->swapChainImages.size());


    std::vector<VkDescriptorSetLayout> layouts(
        ctx.swapChain->swapChainImages.size(),
        postProcessingLayout
    );

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = ctx.swapChain->swapChainImages.size();
    allocInfo.pSetLayouts = layouts.data();

    if (vkAllocateDescriptorSets(*ctx.device, &allocInfo, postProcessingSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate camera descriptor sets");
    }

    std::cout << "[DESCRIPTORSET] ALOCATED" << std::endl;

    for (uint32_t i = 0; i < ctx.swapChain->swapChainImages.size(); i++)
    {
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = images[i]->textureImageView->imageView;
        imageInfo.sampler = samplers[i]->textureSampler;
        std::cout << "[DESCRIPTORSET] TEXTURE" << std::endl;

        std::array<VkWriteDescriptorSet, 1> writes{};

        writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[0].dstSet = postProcessingSets[i];
        writes[0].dstBinding = 0;
        writes[0].dstArrayElement = 0;
        writes[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writes[0].descriptorCount = 1;
        writes[0].pImageInfo = &imageInfo;


        vkUpdateDescriptorSets(*ctx.device, static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
        std::cout << "[DESCRIPTORSET] FRAME DONE" << std::endl;
    }
    std::cout << "[DESCRIPTORSET] DONE" << std::endl;

}



void DescriptorManager::setObjectDescriptor(Object3D &object, Camera3D & camera)
{
    object.descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);


    std::vector<VkDescriptorSetLayout> layouts(
        MAX_FRAMES_IN_FLIGHT,
        descriptorSetLayout
    );

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
    allocInfo.pSetLayouts = layouts.data();

    if (vkAllocateDescriptorSets(*ctx.device, &allocInfo, object.descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate camera descriptor sets");
    }

    std::cout << "[DESCRIPTORSET] ALOCATED" << std::endl;

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = camera.uniformBuffers[i]->buffer;
        bufferInfo.offset = 0;
        bufferInfo.range  = sizeof(CameraBufferObject);
        std::cout << "[DESCRIPTORSET] CAMERA" << std::endl;

        VkDescriptorBufferInfo bufferInfoTransform{};
        bufferInfoTransform.buffer = object.transformBuffers[i]->buffer;
        bufferInfoTransform.offset = 0;
        bufferInfoTransform.range  = sizeof(Transform3D);
        std::cout << "[DESCRIPTORSET] TRANSFORM" << std::endl;

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = object.texture->image->textureImageView->imageView;
        imageInfo.sampler = object.texture->sampler->textureSampler;
        std::cout << "[DESCRIPTORSET] TEXTURE" << std::endl;

        std::array<VkWriteDescriptorSet, 3> writes{};

        writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[0].dstSet = object.descriptorSets[i];
        writes[0].dstBinding = 0; // camera binding
        writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writes[0].descriptorCount = 1;
        writes[0].pBufferInfo = &bufferInfo;

        writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[1].dstSet = object.descriptorSets[i];
        writes[1].dstBinding = 1;
        writes[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writes[1].descriptorCount = 1;
        writes[1].pBufferInfo = &bufferInfoTransform;

        writes[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[2].dstSet = object.descriptorSets[i];
        writes[2].dstBinding = 2;
        writes[2].dstArrayElement = 0;
        writes[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writes[2].descriptorCount = 1;
        writes[2].pImageInfo = &imageInfo;


        vkUpdateDescriptorSets(*ctx.device, static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
        std::cout << "[DESCRIPTORSET] FRAME DONE" << std::endl;
    }
    std::cout << "[DESCRIPTORSET] DONE" << std::endl;

}







void DescriptorManager::bindCameraBuffers(std::vector<Buffer*>& cameraBuffers)
{
    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = cameraBuffers[i]->buffer;
        bufferInfo.offset = 0;
        bufferInfo.range  = sizeof(CameraBufferObject);

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = sets[i];
        write.dstBinding = 0; // camera binding
        write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        write.descriptorCount = 1;
        write.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(*ctx.device, 1, &write, 0, nullptr);
    }
}

void DescriptorManager::bindTextureData(std::vector<Texture*> &textures)
{
    std::vector<VkDescriptorImageInfo> imageInfos;
    imageInfos.resize(textures.size());

    for (size_t i = 0; i < textures.size(); i++) {
        imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfos[i].imageView   = textures[i]->image->textureImageView->imageView;
        imageInfos[i].sampler     = textures[i]->sampler->textureSampler;
    }
    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = sets[i];
        write.dstBinding = 2; // bindless texture binding
        write.dstArrayElement = 0;
        write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write.descriptorCount = static_cast<uint32_t>(imageInfos.size());
        write.pImageInfo = imageInfos.data();

        vkUpdateDescriptorSets(*ctx.device, 1, &write, 0, nullptr);
    }
    std::cout << "[DESCRIPTORSET] DONE" << std::endl;
}

void DescriptorManager::bindObjectBuffers(std::vector<Buffer*> &objectBuffers)
{
    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = objectBuffers[i]->buffer;
        bufferInfo.offset = 0;
        bufferInfo.range  = sizeof(Transform3D) * MAX_OBJECTS;

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = sets[i];
        write.dstBinding = 1; // objects binding
        write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        write.descriptorCount = 1;
        write.pBufferInfo = &bufferInfo;
        vkUpdateDescriptorSets(*ctx.device, 1, &write, 0, nullptr);
    }
}

DescriptorManager::~DescriptorManager()

{

    vkDestroyDescriptorSetLayout(*ctx.device, descriptorSetLayout, nullptr);
    vkDestroyDescriptorPool(*ctx.device, descriptorPool, nullptr);


}