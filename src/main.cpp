#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <fstream>
#include <vector>
#include <array>

#include "shader.hpp"
#include "camera.hpp"

static const int WIDTH = 800;
static const int HEIGHT = 600;
static const float NEAR_PLANE = 0.1;
static const float FAR_PLANE = 1000.;

typedef struct GlobalState {
    GLFWwindow* window;
    Camera camera;
    int world_scale = 4;

    GlobalState() {}
    ~GlobalState() {}
    
    void free() {
        glfwDestroyWindow(window);
    }
} GlobalState;
GlobalState gstate;





void on_mouse_move(GLFWwindow* window, double x,double y) {
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



struct Vertex {
    float x, y, z;
};


int main()
{
    gstate.window = glfw_init(WIDTH, HEIGHT);
    glfwSetInputMode(gstate.window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);



    std::vector<float> height_map_data = load_height_map("res/heightmap.data");
    int height_map_dim = std::sqrt(height_map_data.size());
    std::vector<uint32_t> indices;

    for (size_t z = 0; z < height_map_dim - 1; z++) {
        for (size_t x = 0; x < height_map_dim - 1; x++) {
            indices.push_back(z * height_map_dim + x);
            indices.push_back((z + 1) * height_map_dim + x);
            indices.push_back((z + 1) * height_map_dim + x + 1);

            indices.push_back(z * height_map_dim + x);
            indices.push_back((z + 1) * height_map_dim + x + 1);
            indices.push_back(z * height_map_dim + x + 1);
        }
    }
    

    std::vector<Vertex> points;
    for (int x = 0; x < height_map_dim; x++) {
        for (int z = 0; z < height_map_dim; z++) {
            points.push_back((Vertex) {
                .x =  (float)(x - height_map_dim / 2) * gstate.world_scale ,
                .y = height_map_data[x +z * height_map_dim],
                .z =  (float)(z - height_map_dim / 2) * gstate.world_scale,
            });
        }
    }

    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
    Shader shader;
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(points[0]), points.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glEnableVertexAttribArray(0);


        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        shader.read_files("src/shaders/shader.vert", "src/shaders/shader.frag");
    
        shader.enable();
        glm::mat4 mtx_proj = glm::perspective<float>(glm::radians(45.f),(float) WIDTH / HEIGHT,NEAR_PLANE,FAR_PLANE);
        glm::mat4 mtx_model = glm::translate(glm::mat4(1),glm::vec3(0,0,0));

        shader.set_mat4x4("mtx_proj",glm::value_ptr(mtx_proj));
        shader.set_mat4x4("mtx_model",glm::value_ptr(mtx_model));        
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


        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,indices.data());

        glfwSwapBuffers(gstate.window);
        glClearColor(0., 0., 0., 1.);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }


    gstate.free();
    glfw_exit();
    return 0;
}