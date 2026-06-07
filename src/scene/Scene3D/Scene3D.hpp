



#pragma once

#include "scene/Scene.hpp"


class Scene3D {
    public:

        std::vector<Object3D *>             objects;
        Camera3D                          camera;



        Scene3D(VulkanContext &ctx) : ctx(ctx), camera(Camera3D(ctx)){
            testScene();
        };
        ~Scene3D();
        void update(uint32_t currentImage) {
            camera.update(currentImage);
            for (Object3D *object : objects)
            {
                object->update(currentImage);
            }
        };
        void testScene();
        
    private:
        VulkanContext &ctx;
};