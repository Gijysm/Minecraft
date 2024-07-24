#ifndef BLOCK_H
#define BLOCK_H
#include <cstdint>
class Block
{
public:
	static Block* blocks[256];


	Block(unsigned int id, int16_t texture);
	~Block();

	const unsigned int id;
	int textureFaces[6];
	unsigned char emission[3];
};

#endif // !BLOCK_H