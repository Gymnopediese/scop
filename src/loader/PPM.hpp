#pragma once

#include <fstream>
#include <vector>
#include <string>

class PPM {
    public:
        int width;
        int height;
        int success;
        int size;
        int depth;

        unsigned int *pixels;
        PPM(const std::string& path);
        ~PPM();
};
