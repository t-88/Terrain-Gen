#pragma once
#include <stdint.h>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Shader
{
private:
    uint32_t ID;
    uint32_t vertex_shader;
    uint32_t fragment_shader;

public:
    Shader();
    Shader(std::string,std::string);
    ~Shader();



    void enable();
    void free();

    void read_files(std::string,std::string);
    void set_bool(std::string name, bool val);
    void set_int(std::string name, int val);
    void set_float(std::string name, float val);
    void set_vec3(std::string name, std::vector<float> val);
    void set_mat4x4(std::string name, GLfloat* mat_ptr);
};

