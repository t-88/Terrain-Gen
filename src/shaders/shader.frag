#version 330 core 

in vec3 frag_pos;

out vec4 FragColor;


void main() {
    FragColor = vec4(vec3(frag_pos.y) / 200,1.);
}