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

    ~Camera();
    Camera();
    Camera(glm::vec3 _pos,glm::vec3 _front,glm::vec3 _up);

    void on_key_press(GLFWwindow* window);
    void on_mouse_move(float x, float y);
    glm::mat4 get_view_matrix();
};
