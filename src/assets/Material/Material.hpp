#pragma once

#include "assets/Assets.hpp"


class Texture;

class Material {
    public:
        Pipeline                *pipeline;

        Material(VulkanContext &ctx);
        ~Material();

    private:
        VulkanContext &ctx;

};