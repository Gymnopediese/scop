
#include "PPM.hpp"

void Image::loadPPM(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);

    if (!file)
        throw std::runtime_error("Failed to open file: " + path);

    std::string magic;
    file >> magic;

    if (magic != "P6")
        throw std::runtime_error("Unsupported PPM format (expected P6)");

    int maxValue;
    file >> width >> height >> maxValue;

    if (!file)
        throw std::runtime_error("Invalid PPM header");

    if (width <= 0 || height <= 0)
        throw std::runtime_error("Invalid image dimensions");

    if (maxValue != 255)
        throw std::runtime_error("Only max value 255 is supported");

    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    const size_t pixelCount =
        static_cast<size_t>(width) *
        static_cast<size_t>(height) * 3;

    // Overflow check
    if (pixelCount / 3 !=
        static_cast<size_t>(width) * static_cast<size_t>(height))
    {
        throw std::runtime_error("Image dimensions are too large");
    }

    pixels.resize(pixelCount);

    file.read(reinterpret_cast<char*>(pixels.data()),
              static_cast<std::streamsize>(pixelCount));

    if (file.gcount() != static_cast<std::streamsize>(pixelCount))
        throw std::runtime_error("Unexpected end of file while reading pixels");

    // Detect trailing garbage
    char extra;
    if (file.read(&extra, 1))
        throw std::runtime_error("File contains extra data after image");
    size = width * height * 4;

}

Image::~Image()
{
    pixels.clear();
    pixels.shrink_to_fit();

    width = 0;
    height = 0;
}