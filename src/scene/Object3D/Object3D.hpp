
#pragma once

#include "assets/Mesh/Mesh.hpp"
#include "main.hpp"
#include "loader/OBJ.hpp"


class Object3D {
    public:

        Mesh                            *mesh;
        Material                        *material;
        Texture                         *texture;
        Transform3D                     transform;

        std::vector<VkDescriptorSet>    descriptorSets;
        std::vector<Buffer*>            transformBuffers;
        glm::vec3                       minBounds = glm::vec3( FLT_MAX);
        glm::vec3                       maxBounds = glm::vec3(-FLT_MAX);

        Object3D(){};
        ~Object3D(){};



        static Object3D *loadOBJ(VulkanContext &ctx, const std::string &path, const std::string &texture = ""){
            const std::vector<glm::vec2> default_vt = {
                {1, 0},
                {0, 0},
                {0, 1},
                {1, 1}
            };
            Object3D *object = new Object3D();
            object->mesh = new Mesh();
            if (texture != "")
            {
                object->texture = Texture::load_ppm(texture, ctx);
            }

            ObjLoader::Mesh mesh = ObjLoader::loadOBJ(path);
            
            std::cout << "yout litle freaky boy " << std::endl;
            int default_index = 0;



            for (ObjLoader::Face face : mesh.faces)
            {
                for (const auto& index : face.indices)
                {
                    Vertex vertex{};
                    object->minBounds.x = std::min(object->minBounds.x, mesh.vertices[index.v].x);
                    object->minBounds.y = std::min(object->minBounds.y, mesh.vertices[index.v].y);
                    object->minBounds.z = std::min(object->minBounds.z, mesh.vertices[index.v].z);

                    object->maxBounds.x = std::max(object->maxBounds.x, mesh.vertices[index.v].x);
                    object->maxBounds.y = std::max(object->maxBounds.y, mesh.vertices[index.v].y);
                    object->maxBounds.z = std::max(object->maxBounds.z, mesh.vertices[index.v].z);

                    vertex.pos = {
                        mesh.vertices[index.v].x,
                        mesh.vertices[index.v].y,
                        mesh.vertices[index.v].z
                    };
                    if (index.vt < mesh.uvs.size())
                        vertex.uv = {
                            mesh.uvs[index.vt].x,
                            mesh.uvs[index.vt].y
                        };
                    else
                    {
                        // vertex.uv = default_vt[default_index];
                        // default_index += 1;
                        vertex.uv = {
                            mesh.vertices[index.v].x,
                            1 - mesh.vertices[index.v].y,
                        };


                        vertex.uv = {
                            -100, -100
                        };
                    }
                    
                    object->mesh->vertices.push_back(vertex);
                    object->mesh->indices.push_back(object->mesh->indices.size());
                }


            }

            for (Vertex &v :  object->mesh->vertices)
            {
                if (v.uv.x == -100)
                {
                    v.uv = {
                        (v.pos.x - object->minBounds.x) / (object->maxBounds.x - object->minBounds.x),
                        (v.pos.y - object->minBounds.y) / (object->maxBounds.y - object->minBounds.y)
                    };
                }
            }

            return object;
        };

        void color(glm::vec3 color)
        {
            int i = 0;
            for (Vertex &v : mesh->vertices)
            {
                v.modulate = color;
                float mult = (int(i / 3 + 1) * (v.pos.z - minBounds.z) / (maxBounds.z - minBounds.z)) / int(mesh->vertices.size() / 3);
                // mult = mult * 0.7 + 0.3;
                v.modulate *= mult;
                i += 1;
            }
        };


        void update(const int imageIndex) {
            transformBuffers[imageIndex]->mapMemory(&transform, sizeof(Transform3D));
        };

};