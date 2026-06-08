#pragma once

#include "assets/Assets.hpp"
#include <vector>

class Texture {
    public:

        static std::vector<Texture *> all_textures;


        Image   *image;
        Sampler *sampler;
        u32     index;


        static Texture *load_ppm(const std::string &path, VulkanContext &ctx);
        Texture(){};
        Texture& operator=(const Texture& other);
        ~Texture();
        
};
