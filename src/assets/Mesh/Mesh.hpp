#pragma once

#include "assets/Assets.hpp"

class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        Buffer *vertexBuffer;
        Buffer *indexBuffer;

        Mesh(){};
        ~Mesh(){
            delete vertexBuffer;
            delete indexBuffer;
        };
        
        void setVertices(std::vector<Vertex> &vertices);

};