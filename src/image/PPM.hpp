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
        int depth;

        unsigned int *pixels;
        void loadPPM(const std::string& path);
        Image(){};
        ~Image();
};
