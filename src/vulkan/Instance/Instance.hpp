#pragma once

#include "main.hpp"

class Instance
{
public:

    Instance(VulkanContext &ctx);
    ~Instance();

private:
    VkInstance                      instance;
    
};
