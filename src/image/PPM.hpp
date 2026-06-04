#pragma once

#include <fstream>
#include <vector>
#include <string>

class Image {
    public:
        int width;
        int height;
        int success;
        int size;

        std::vector<unsigned char> pixels;
        void loadPPM(const std::string& path);
        Image(){};
        ~Image();
};
