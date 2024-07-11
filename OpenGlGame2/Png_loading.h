#ifndef PNG_LOADING_H
#define PNG_LOADING_H
#include "lib.h"
#include "Texture.h"
#include "Defines.h"
class Texture;
GLuint png_load(const char* file, int* width, int* height, int* nrComponents);
Texture* load_texture(const std::string& file);
class Png_loading
{
};

#endif // !PNG_LOADING_H