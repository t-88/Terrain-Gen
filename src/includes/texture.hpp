#pragma once
#include <stdint.h>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

class Texture
{
public:
    uint32_t ID;
    std::string type;
    std::string path;

    Texture();
    Texture(const char* fp,int out_comps = 3);
    void from_file(const char* fp,int out_comps = 3);
    ~Texture();
    void bind(int placement = 0);
};

