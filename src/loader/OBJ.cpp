#include "OBJ.hpp"

namespace ObjLoader {


 
static FaceIndex parseFaceVertex(const std::string& token) {
    FaceIndex idx;

    std::stringstream ss(token);
    std::string v, vt, vn;

    std::getline(ss, v, '/');
    std::getline(ss, vt, '/');
    std::getline(ss, vn, '/');

    if (!v.empty()) idx.v = std::stoi(v) - 1;
    if (!vt.empty()) idx.vt = std::stoi(vt) - 1;
    if (!vn.empty()) idx.vn = std::stoi(vn) - 1;

    return idx;
}

Mesh loadOBJ(const std::string& path) {
    Mesh mesh;
    std::ifstream file(path);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open OBJ file");
    }

    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            Vec3 v;
            ss >> v.x >> v.y >> v.z;
            mesh.vertices.push_back(v);
        }
        else if (type == "vt") {
            Vec2 vt;
            ss >> vt.x >> vt.y;
            mesh.uvs.push_back(vt);
        }
        else if (type == "vn") {
            Vec3 vn;
            ss >> vn.x >> vn.y >> vn.z;
            mesh.normals.push_back(vn);
        }
        else if (type == "f") {
            Face face;
            std::string token;

            std::vector<FaceIndex> faceVertices;

            while (ss >> token) {
                faceVertices.push_back(parseFaceVertex(token));
            }

            // triangulate (fan method)
            for (size_t i = 1; i + 1 < faceVertices.size(); i++) {
                Face tri;
                tri.indices.push_back(faceVertices[0]);
                tri.indices.push_back(faceVertices[i]);
                tri.indices.push_back(faceVertices[i + 1]);
                mesh.faces.push_back(tri);
            }
        }
    }

    return mesh;
}

}