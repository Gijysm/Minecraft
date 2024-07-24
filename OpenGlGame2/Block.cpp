#include "Block.h"
Block* Block::blocks[256] = { nullptr };
Block::Block(unsigned int id, int16_t texture) : id(id),
    textureFaces{ texture, texture, texture, texture, texture, texture }, emission{0,0,0}{

}
