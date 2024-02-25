#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <vector>
#include <array>

#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "vertex.hpp"
#include "terrain_generator.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static const int WIDTH = 800;
static const int HEIGHT = 600;
static const float NEAR_PLANE = 0.1;
static const float FAR_PLANE = 1000.;

typedef struct GlobalState {
    GLFWwindow* window;
    Camera camera;
    int world_scale = 2;

    GlobalState() {}
    ~GlobalState() {}
    
    void free() {
        glfwDestroyWindow(window);
    }
} GlobalState;
GlobalState gstate;






void on_mouse_move(GLFWwindow* window, double x,double y) 
{
    gstate.camera.on_mouse_move(x,y);
}
GLFWwindow *glfw_init(int width, int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow *window = glfwCreateWindow(width, height, "Terrian-Gen", 0, 0);
    if (window == NULL)
    {
        printf("%s:%d Failed to initialize the window\n", __FILE__, __LINE__);
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("%s:%d Failed to initialize the window\n", __FILE__, __LINE__);
        glfwTerminate();
        exit(-1);
    }
    glViewport(0, 0, width, height);

    glfwSetCursorPosCallback(window,on_mouse_move);

    return window;
}
void glfw_exit()
{
    glfwTerminate();
}
std::vector<float> load_height_map(const char *path)
{
    std::ifstream file_stream(path, std::ios::binary | std::ios::ate);
    uint32_t file_size = file_stream.tellg();
    if (file_size % sizeof(float) != 0)
    {
        printf("%s:%d Possible Corruption: file should only contain floats \n", __FILE__, __LINE__);
        file_stream.close();
        exit(-1);
    }

    file_stream.seekg(std::ios_base::beg);
    char *file_bytes = (char *)malloc(file_size);
    file_stream.read(file_bytes, file_size);
    file_stream.close();

    float *file_floats = (float *)(file_bytes);
    std::vector<float> file_data;
    file_data.assign(file_floats, file_floats + file_size / sizeof(float));
    free(file_bytes);

    return file_data;
}

std::vector<uint32_t> generate_indices(int terrain_size) {
    std::vector<uint32_t> indices;
    for (size_t z = 0; z < terrain_size - 1; z++) {
        for (size_t x = 0; x < terrain_size - 1; x++) {
            indices.push_back(z * terrain_size + x);
            indices.push_back((z + 1) * terrain_size + x);
            indices.push_back((z + 1) * terrain_size + x + 1);

            indices.push_back(z * terrain_size + x);
            indices.push_back((z + 1) * terrain_size + x + 1);
            indices.push_back(z * terrain_size + x + 1);
        }
    }
    return indices;
}
std::vector<glm::vec3> generat_normals_from_indices(std::vector<uint32_t> indices,std::vector<glm::vec3> positions) {
    std::vector<glm::vec3> normals(positions.size());
    for (size_t i = 0; i < indices.size() / 3; i++) {
        int a_idx = indices[3 * i + 0];
        int b_idx = indices[3 * i + 1];
        int c_idx = indices[3 * i + 2];

        glm::vec3 a = positions[a_idx];
        glm::vec3 b = positions[b_idx];
        glm::vec3 c = positions[c_idx];
    
        glm::vec3 ab = a - b;
        glm::vec3 ac = a - c;
        glm::vec3 normal = glm::cross(ab,ac);
        
        normals[a_idx] = normal;
        normals[b_idx] = normal;
        normals[c_idx] = normal;
    }    
    return normals;
}
std::vector<Vertex> generate_vertex_buffer(int terrain_size,std::vector<glm::vec3> positions,std::vector<glm::vec3> normals,std::vector<glm::vec2> texture_coords) {
    std::vector<Vertex> vertices(terrain_size * terrain_size);
    for (size_t i = 0; i < terrain_size * terrain_size; i++) {
        vertices[i] = (Vertex){
            .x = positions[i].x,
            .y = positions[i].y,
            .z = positions[i].z,

            .nx = normals[i].x,
            .ny = normals[i].y,
            .nz = normals[i].y,

            .u = texture_coords[i].x,
            .v = texture_coords[i].y,
        };
    }
    return vertices;
} 
int main()
{
    srand(time(0));
    
    gstate.window = glfw_init(WIDTH, HEIGHT);
    glfwSetInputMode(gstate.window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(gstate.window,WIDTH / 2.f,HEIGHT / 2.f);

    float terrain_max_h = 200 , terrain_min_h = 0;
    int terrain_size = 200;
    std::vector<float> height_map_data = TerrainGenerator::gen_fault_formation(terrain_size,200,terrain_max_h,terrain_min_h,0.8); 


    

    std::vector<glm::vec3> positions(terrain_size*terrain_size,glm::vec3(0,0,0));
    std::vector<glm::vec2> texture_coords(terrain_size*terrain_size,glm::vec2(0));
    std::vector<uint32_t> indices;
    std::vector<glm::vec3> normals;
    std::vector<Vertex> points;
    {
        for (int x = 0; x < terrain_size; x++) {
            for (int z = 0; z < terrain_size; z++) {
                positions[x + z * terrain_size].x =  (float)(x - terrain_size / 2) * gstate.world_scale;
                positions[x + z * terrain_size].y = height_map_data[x +z * terrain_size];
                positions[x + z * terrain_size].z =  (float)(z - terrain_size / 2) * gstate.world_scale;
            
                texture_coords[x + z * terrain_size].x = (x / (float)terrain_size);
                texture_coords[x + z * terrain_size].y = (z / (float)terrain_size);
            }
        }

        indices = generate_indices(terrain_size);
        normals = generat_normals_from_indices(indices,positions);
        points = generate_vertex_buffer(terrain_size,positions,normals,texture_coords);        
    }
   
    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
    Shader shader;
    Texture snow_texture;
    Texture grass_texture;
    Texture desert_texture;
    Texture ground_texture;
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(points[0]), points.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        shader.read_files("src/shaders/shader.vert", "src/shaders/shader.frag");
    
        shader.enable();
        glm::mat4 mtx_proj = glm::perspective<float>(glm::radians(45.f),(float) WIDTH / HEIGHT,NEAR_PLANE,FAR_PLANE);
        glm::mat4 mtx_model = glm::translate(glm::mat4(1),glm::vec3(0,0,0));

        shader.set_mat4x4("mtx_proj",glm::value_ptr(mtx_proj));
        shader.set_mat4x4("mtx_model",glm::value_ptr(mtx_model));        
        shader.set_float("max_h",terrain_max_h);        
        shader.set_float("min_h",terrain_min_h - 150); 

        glm::vec3 light_dir = glm::normalize(glm::vec3(1,1,1));
        shader.set_vec3("light_dir",{light_dir[0],light_dir[1],light_dir[2]}); 


        snow_texture.from_file("res/snow.png");
        grass_texture.from_file("res/grass_seamless.png");
        desert_texture.from_file("res/desert_seamless.jpg");
        ground_texture.from_file("res/ground_seamless.jpg");
    
        shader.set_int("snow_texture",0);
        shader.set_int("grass_texture",1);
        shader.set_int("ground_texture",2);
        shader.set_int("desert_texture",3);

    }

    gstate.camera = Camera(glm::vec3(0,300,10),glm::vec3(0,-1,-1),glm::vec3(0,1,0));

    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    while (!glfwWindowShouldClose(gstate.window))
    {
        glfwPollEvents();
        if (glfwGetKey(gstate.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(gstate.window, true);
        }
        gstate.camera.on_key_press(gstate.window);


        glm::mat4 mtx_view = gstate.camera.get_view_matrix();
        
        shader.enable();
        shader.set_mat4x4("mtx_view",glm::value_ptr(mtx_view));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,snow_texture.ID);        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,grass_texture.ID);        
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,ground_texture.ID);        
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,desert_texture.ID);        

        glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,indices.data());
        glBindVertexArray(vao);

        glfwSwapBuffers(gstate.window);
        glClearColor(0., 0., 0., 1.);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }


    gstate.free();
    glfw_exit();
    return 0;
}