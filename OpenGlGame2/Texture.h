#ifndef TEXTURE_H
#define TEXTURE_H
#include "lib.h"
#include "Defines.h"
class Texture
{
private:
    GLuint id;
    int width, height;
public:
    Texture(GLuint id, int width, int height);
    ~Texture();

    void bind();

};

#endif // TEXTURE_H
