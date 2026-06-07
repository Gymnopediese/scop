
#include "main.hpp"

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>


namespace ObjLoader {

struct Vec2 {
    float x, y;
};

struct Vec3 {
    float x, y, z;
};

struct FaceIndex {
    int v = -1;
    int vt = -1;
    int vn = -1;
};

struct Face {
    std::vector<FaceIndex> indices;
};

struct Mesh {
    
    std::vector<Vec3> vertices;
    std::vector<Vec2> uvs;
    std::vector<Vec3> normals;
    std::vector<Face> faces;

};


    Mesh loadOBJ(const std::string& path);

}
