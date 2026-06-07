
#include "Material.hpp"

Material::Material(VulkanContext &ctx) : ctx(ctx)
{
    pipeline = 0;
}

Material::~Material()
{
    if (pipeline) delete pipeline;
}