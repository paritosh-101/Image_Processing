#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define BYTE_BOUND(value) value < 0 ? 0 : (value > 255 ? 255 : value)
#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"

Image::Image(const char* filename)
{
    if(read(filename))
    {
        printf("Read %s\n", filename);
        size = w*h*channels;
    }
    else
    {
        printf("Failed to read %s\n", filename);
    }
}
Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels)
{
    size = w*h*channels;
    data = new uint8_t[size];
}
Image::Image(const Image& img) : Image(img.w, img.h, img.channels)
{
    memcpy(data, img.data, size);
}
Image::~Image()
{
    stbi_image_free(data);
}

bool Image::read(const char* filename)
{
    data = stbi_load(filename, &w, &h, &channels, 0);
    return data != NULL;
}
bool Image::write(const char* filename)
{
    ImageType type = getFileType(filename);
    int success;
    switch(type)
    {
        case PNG:
        success = stbi_write_png(filename, w, h, channels, data, w*channels);
        break;

        case JPG:
        success = stbi_write_jpg(filename, w, h, channels, data, 100);
        break;

        case BMP:
        success = stbi_write_bmp(filename, w, h, channels, data);
        break;

        case TGA:
        success = stbi_write_tga(filename, w, h, channels, data);
        break;
    }
    return success != 0;
}

ImageType Image::getFileType(const char* filename)
{
    const char* ext = strchr(filename, '.');
    if(ext != nullptr)
    {
        if(strcmp(ext, ".png") == 0)
        {
            return PNG;
        }
        else if(strcmp(ext, ".jpg") == 0)
        {
            return JPG;
        }
        else if(strcmp(ext, ".bmp") == 0)
        {
            return BMP;
        }
        else if(strcmp(ext, ".tga") == 0)
        {
            return TGA;
        }
    }
    return PNG;
}

Image& Image::grayscale_avg()
{
    // r+g+b/3
    if(channels < 3)
    {
        printf("Image %p has less than 3 channels, it is assumed to already be grayscale.", this);
    }
    else
    {
        for(int i=0; i<size; i+=channels)
        {
            int gray = (data[i] + data[i+1] + data[i+2])/3;
            memset(data+i, gray, 3);
        }
    }
    return *this;
}

Image& Image::grayscale_lum()
{
    if(channels < 3)
    {
        printf("Image %p has less than 3 channels, it is assumed to already be grayscale.", this);
    }
    else
    {
        for(int i=0; i<size; i+=channels)
        {
            int gray = 0.2126*data[i] + 0.7152*data[i+1] + 0.0722*data[i+2];
            memset(data+i, gray, 3);
        }
    }
    return *this;
}

Image& Image::colorMask(float r, float g, float b)
{
    if(channels < 3)
    {
        printf("\e[31m[ERROR] Clor mask requires at least 3 channels, but this image has %d channels\e[0m\n", channels);
    }
    else
    {
        for(int i=0; i < size; i+=channels)
        {
            data[i] *= r;
            data[i+1] *= g;
            data[i+2] *= b;
        }
    }
    return *this;
}

Image& Image::diffmap(Image& img)
{
    int compare_width = fmin(w, img.w);
    int compare_height = fmin(h, img.h);
    int compare_channels = fmin(channels, img.channels);

    for(uint32_t i=0; i<compare_height; ++i)
    {
        for(uint32_t j=0; j<compare_width; ++j)
        {
            for(uint8_t k=0; k<compare_channels; ++k)
            {
                data[(i*w+j)*channels+k] = BYTE_BOUND(abs(data[(i*w+j)*channels+k] - img.data[(i*img.w+j)*img.channels+k]));
            }
        }
    }
    return *this;
}

Image& Image::diffmap_scale(Image& img, uint8_t scl)
{
    int compare_width = fmin(w, img.w);
    int compare_height = fmin(h, img.h);
    int compare_channels = fmin(channels, img.channels);
    uint8_t largest = 0;

    for(uint32_t i=0; i<compare_height; ++i)
    {
        for(uint32_t j=0; j<compare_width; ++j)
        {
            for(uint8_t k=0; k<compare_channels; ++k)
            {
                data[(i*w+j)*channels+k] = BYTE_BOUND(abs(data[(i*w+j)*channels+k] - img.data[(i*img.w+j)*img.channels+k]));
                largest = fmax(largest, data[(i*w+j)*channels+k]);
            }
        }
    }
    scl = 255/fmax(1, fmax(scl, largest));
    for(int i=0; i<size; ++i)
    {
        data[i] *= scl;
    }
    return *this;
}