#version 330 core 

layout (location = 0) in vec3 vert_pos;





uniform mat4 mtx_proj;
uniform mat4 mtx_view;
uniform mat4 mtx_model;

void main() {
    gl_Position = mtx_proj * mtx_view * mtx_model * vec4(vert_pos,1.);
}