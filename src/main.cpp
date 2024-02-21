#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>


static const int WIDTH = 800;
static const int HEIGHT = 600;


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT,"Terrian-Gen",0,0);
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
    glViewport(0,0,WIDTH,HEIGHT);


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window,true);
        }


        glfwSwapBuffers(window);
        glClearColor(0.5,0.5,0.5,1);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0; 
}