#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <fstream>
#include <vector>


static const int WIDTH = 800;
static const int HEIGHT = 600;



GLFWwindow* glfw_init(int width,int height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(width,height,"Terrian-Gen",0,0);
    if(window == NULL) {
        printf("Failed to initialize the window\n");
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("Failed to initialize the window\n");
        glfwTerminate();
        exit(-1);
    }
    glViewport(0,0,width,height);

    return window;
}
void glfw_exit(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}
std::vector<float> load_height_map(const char* path) {
    std::ifstream file_stream(path,std::ios::binary | std::ios::ate);
    uint32_t file_size = file_stream.tellg();
    file_stream.seekg(std::ios_base::beg);
    char* file_bytes = (char*)malloc(file_size); 
    file_stream.read(file_bytes,file_size);
    file_stream.close();

    float* file_floats = (float*)(file_bytes);
    std::vector<float> file_data;
    file_data.assign(file_floats,file_floats + file_size / sizeof(float));
    free(file_bytes);

    return file_data;
}



int main() {
    GLFWwindow* window = glfw_init(WIDTH,HEIGHT);
    std::vector<float> height_map_data = load_height_map("res/heightmap.data");
    

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window,true);
        }


        glfwSwapBuffers(window);
        glClearColor(0.5,0.5,0.5,1);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    

    glfw_exit(window);
    return 0; 
}