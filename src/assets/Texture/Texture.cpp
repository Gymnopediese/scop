
#include "Texture.hpp"

std::vector<Texture*> Texture::all_textures;

Texture& Texture::operator=(const Texture& other) {
    if (this == &other) return *this;  
    image  = other.image;
    sampler = other.sampler;
    return *this; 
}

Texture *Texture::load_ppm(const std::string &path, VulkanContext &ctx)
{
    Texture *res = new Texture();

    res->image = Image::loadPPM(path, ctx);
    res->sampler = new Sampler(ctx);
    res->index = all_textures.size();
    std::cout << "[TEXTURE] " << path << "LOADED" << std::endl;
    return res;
}

Texture::~Texture()
{
    delete sampler;
    delete image;
}




// void Texture::init()
// {
 
//     VkDescriptorSetAllocateInfo allocInfo{};
//     allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
//     allocInfo.descriptorPool = descriptorPool;
//     allocInfo.descriptorSetCount = 1;
//     allocInfo.pSetLayouts = &descriptorSetLayout;

//     descriptorSets.resize(1);

//     if (vkAllocateDescriptorSets(*ctx.device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
//         throw std::runtime_error("failed to allocate bindless descriptor set");
//     }
 
//     // std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);
//     // VkDescriptorSetAllocateInfo allocInfo{};
//     // allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
//     // allocInfo.descriptorPool = descriptorPool;
//     // allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
//     // allocInfo.pSetLayouts = layouts.data();

//     // descriptorSets.resize(swapChainImages.size());
//     // if (vkAllocateDescriptorSets(*ctx.device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
//     //     throw std::runtime_error("echec de l'allocation d'un set de descripteurs!");
//     // }

//     // for (size_t i = 0; i < swapChainImages.size(); i++) {
//     //     VkDescriptorBufferInfo bufferInfo{};
//     //     bufferInfo.buffer = uniformBuffers[i]->buffer;
//     //     bufferInfo.offset = 0;
//     //     bufferInfo.range = sizeof(UniformBufferObject);

//     //     VkDescriptorImageInfo imageInfo{};
//     //     imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//     //     imageInfo.imageView = image->textureImageView->imageView;
//     //     imageInfo.sampler = image->textureSampler;

//     //     std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

//     //     descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//     //     descriptorWrites[0].dstSet = descriptorSets[i];
//     //     descriptorWrites[0].dstBinding = 0;
//     //     descriptorWrites[0].dstArrayElement = 0;
//     //     descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//     //     descriptorWrites[0].descriptorCount = 1;
//     //     descriptorWrites[0].pBufferInfo = &bufferInfo;

//     //     descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//     //     descriptorWrites[1].dstSet = descriptorSets[i];
//     //     descriptorWrites[1].dstBinding = 1;
//     //     descriptorWrites[1].dstArrayElement = 0;
//     //     descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//     //     descriptorWrites[1].descriptorCount = 1;
//     //     descriptorWrites[1].pImageInfo = &imageInfo;

//     //     vkUpdateDescriptorSets(*ctx.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

//     // }

// }