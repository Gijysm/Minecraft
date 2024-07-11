#include "Texture.h"

Texture::Texture(GLuint id, int width, int height)
    : id(id), width(width), height(height) {

}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void Texture::bind()
{
    glActiveTexture(GL_TEXTURE0); // Activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, id);
}

