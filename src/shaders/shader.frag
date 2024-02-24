#version 330 core 

in vec3 frag_pos;
in vec2 frag_tex;

out vec4 FragColor;



uniform sampler2D snow_texture;
uniform sampler2D grass_texture;
uniform sampler2D ground_texture;
uniform sampler2D desert_texture;

const float height0 = 30.f;
const float height1 = 50.f;
const float height2 = 80.f;
const float height3 = 160.f;

uniform float max_h; 
uniform float min_h; 
void main() {

    vec4 color;

    if(frag_pos.y > height3) {
       color = texture(snow_texture, frag_tex);
    }
    else if(frag_pos.y > height2) {
       vec4 Color0 = texture(grass_texture, frag_tex);
       vec4 Color1 = texture(snow_texture, frag_tex);
       float Factor = (frag_pos.y - height2) / (height3 - height2);
       color = mix(Color0, Color1, Factor);
    }
    else if(frag_pos.y > height1) {
       vec4 Color0 = texture(ground_texture, frag_tex);
       vec4 Color1 = texture(grass_texture, frag_tex);
       float Factor = (frag_pos.y - height1) / (height2 - height1);
       color = mix(Color0, Color1, Factor);
    }
    else if(frag_pos.y > height0) {
       vec4 Color0 = texture(desert_texture, frag_tex);
       vec4 Color1 = texture(ground_texture, frag_tex);
       float Factor = (frag_pos.y - height0) / (height1 - height0);
       color = mix(Color0, Color1, Factor);
    } else {
       color = texture(desert_texture, frag_tex);
    }

    FragColor = color *  vec4(vec3(frag_pos.y - min_h) / (max_h - min_h),1.);
}