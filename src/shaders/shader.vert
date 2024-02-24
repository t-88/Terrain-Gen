#version 330 core 

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_tex;




out vec3 frag_pos;
out vec2 frag_tex;

uniform mat4 mtx_proj;
uniform mat4 mtx_view;
uniform mat4 mtx_model;

void main() {
    gl_Position = mtx_proj * mtx_view * mtx_model * vec4(vert_pos,1.);
    frag_pos = vert_pos;
    frag_tex = vert_tex;
}