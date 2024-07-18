#include "Png_loading.h"
#include "Texture.h"

GLuint png_load(const char* file, int* width, int* height, int* nrComponents) {
    // Завантаження зображення за допомогою SOIL2
    unsigned char* image = SOIL_load_image(file, width, height, nrComponents, SOIL_LOAD_RGBA);
    if (image == nullptr) {
        fprintf(stderr, "Error: Could not load image %s\n", file);
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLenum format;
    if (*nrComponents == 1)
        format = GL_RED;
    else if (*nrComponents == 3)
        format = GL_RGB;
    else if (*nrComponents == 4)
        format = GL_RGBA;

    // Завантаження даних зображення в текстуру
    glTexImage2D(GL_TEXTURE_2D, 0, format, *width, *height, 0, format, GL_UNSIGNED_BYTE, image);

    // Встановлення параметрів текстури для піксельного вигляду
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Генерація mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Звільнення пам'яті зображення
    SOIL_free_image_data(image);

    // Відв'язування текстури
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}

Texture* load_texture(const std::string& file)
{
    int width, height, nrComponents;
    GLuint textureID = png_load(file.c_str(), &width, &height, &nrComponents);
    if (textureID == 0)
    {
        std::cerr << "Failed to load texture" << std::endl;
        return nullptr;
    }
    return new Texture(textureID, width, height);
}
