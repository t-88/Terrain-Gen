#version 330 core 

in vec3 frag_pos;
in vec3 frag_norm;
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
uniform vec3 light_dir; 


vec4 mix_textures() {
   vec4 color;
   if(frag_pos.y > height3) {
     color = texture(snow_texture, frag_tex);
   }
   else if(frag_pos.y > height2) {
     vec4 c0 = texture(grass_texture, frag_tex);
     vec4 c1 = texture(snow_texture, frag_tex);
     float delta = (frag_pos.y - height2) / (height3 - height2);
     color = mix(c0, c1, delta);
   }
   else if(frag_pos.y > height1) {
     vec4 c0 = texture(ground_texture, frag_tex);
     vec4 c1 = texture(grass_texture, frag_tex);
     float delta = (frag_pos.y - height1) / (height2 - height1);
     color = mix(c0, c1, delta);
   }
   else if(frag_pos.y > height0) {
     vec4 c0 = texture(desert_texture, frag_tex);
     vec4 c1 = texture(ground_texture, frag_tex);
     float delta = (frag_pos.y - height0) / (height1 - height0);
     color = mix(c0, c1, delta);
   } else {
     color = texture(desert_texture, frag_tex);
   }
   return color;
}

void main() {
   vec4 color = mix_textures();

   vec4 diffuse = vec4(vec3(max(0,dot(light_dir,frag_norm))),1);
   vec4 ambiant = vec4(0.2,0.2,0.2,1);


   FragColor = (diffuse + ambiant) * color;
   // FragColor = vec4(1.f);
}