#include "camera.hpp"

Camera::~Camera() {}

Camera::Camera() : speed(default_speed) {

}
Camera::Camera(glm::vec3 _pos, glm::vec3 _front, glm::vec3 _up) : pos(_pos), front(_front), up(_up) , speed(default_speed) {

}

void Camera::on_key_press(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        speed = default_speed * speed_multiplier;
    }
    else
    {
        speed = default_speed;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        pos -= speed * front;
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        pos += speed * front;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        pos += speed * glm::normalize(glm::cross(front, up));
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        pos -= speed * glm::normalize(glm::cross(front, up));
    }
}

void Camera::on_mouse_move(float x, float y)
{
    static bool first_move = true;
    if (first_move)
    {
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

    if (pitch > 89.)
    {
        pitch = 89.;
    }
    else if (pitch < -89.)
    {
        pitch = -89.;
    }

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
}

glm::mat4 Camera::get_view_matrix()
{
    return glm::lookAt(pos, pos + front, up);
}
