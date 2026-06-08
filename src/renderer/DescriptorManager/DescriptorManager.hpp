



#pragma once

#include "renderer/Renderer.hpp"

class DescriptorManager  {
    public:

        std::vector<VkDescriptorSet>    sets;

        VkDescriptorSet                 texturesSet;
        std::vector<VkDescriptorSet>    cameraSets;
        std::vector<VkDescriptorSet>    objectSets;
        VkDescriptorPool                descriptorPool;

        VkDescriptorSetLayout           descriptorSetLayout;
        VkDescriptorSetLayout           postProcessingLayout;
        std::vector<VkDescriptorSet>    postProcessingSets;
        
        DescriptorManager(VulkanContext &ctx);
        ~DescriptorManager();

        void bindCameraBuffers(std::vector<Buffer *>& cameraBuffers);
        void bindTextureData(std::vector<Texture*>& textures);
        void bindObjectBuffers(std::vector<Buffer*>& objectBuffers);


        void createDescriptorPool();
        void createDescriptorSetLayout();
        void createCameraData();
        void createTexturesData();
        void createObjectsData();

        void createPostProcessLayout();
        void createPostPorcessSets(std::vector<Image *> &images, std::vector<Sampler *> &samplers);
        void deletePostPorcessSets();

        void setObjectDescriptor(Object3D &object, Camera3D & camera);
        

    private:
        VulkanContext &ctx;

};