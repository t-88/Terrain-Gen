#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>



class Camera {
public:
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;

    float default_speed = 0.5f;
    float speed_multiplier = 4;
    float speed;


    float last_x = 0;
    float last_y = 0;

    float yaw , pitch = 0;
    float sensitivity = 0.1;



    ~Camera() {}
    Camera() {
        speed = default_speed;
    }
    Camera(glm::vec3 _pos,glm::vec3 _front,glm::vec3 _up) : pos(_pos),front(_front),up(_up) {
        speed = default_speed;
    }


    void on_key_press(GLFWwindow* window) {
        if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            speed = default_speed  * speed_multiplier;
        } else {
            speed = default_speed;
        }

        if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS) {
            pos -=  speed * front;
        } else if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS) {
            pos +=  speed * front;
        } else if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS) { 
            pos +=  speed * glm::normalize(glm::cross(front,up));
        } else if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS) { 
            pos -=  speed * glm::normalize(glm::cross(front,up));
        }
    }

    void on_mouse_move(float x, float y) {
        static bool first_move = true;
        if(first_move) {
            first_move = false;
            last_x = x;
            last_y = y;
        }

        float x_off = x - last_x; 
        float y_off = y - last_y;
        last_x = x;
        last_y = y;

        yaw += x_off * sensitivity;
        pitch += -y_off * sensitivity;


        if(pitch > 89.) {
            pitch = 89.;
        } else if(pitch < -89.) {
            pitch = -89.;
        }

        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch)); 
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);

    }

    glm::mat4 get_view_matrix() {
        return glm::lookAt(pos,pos + front,up);
    }
};
