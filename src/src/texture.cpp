#include "texture.hpp"

Texture::Texture() {}
Texture::Texture(const char* fp,int out_comps) {
    from_file(fp,out_comps);
}

Texture::~Texture() {}

void Texture::bind(int placement) {
    glActiveTexture(GL_TEXTURE0 + placement);
    glBindTexture(GL_TEXTURE_2D,ID);
}

void Texture::from_file(const char* fp,int out_comps) {
    glGenTextures(1,&ID);

    path = fp;

    glBindTexture(GL_TEXTURE_2D,ID);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    int width , height;
    uint8_t* tex_data = stbi_load(fp,&width,&height,0,out_comps); 

    if(tex_data == 0) {
        printf("%s:%d Failed to load texture '%s'\n",__FILE__,__LINE__,fp);
        exit(-1);
    }

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,tex_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_data);
}