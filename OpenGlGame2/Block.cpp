#include "Block.h"


Block::Block(unsigned int id, int16_t texture) : id(id)
{
    textureFaces[0] = texture;
    textureFaces[1] = texture;
    textureFaces[2] = texture;
    textureFaces[3] = texture;
    textureFaces[4] = texture;
    textureFaces[5] = texture;
}
