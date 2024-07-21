#include "LightMap.h"

LightMap::LightMap()
{
	map = new unsigned short[_CHUNK_SIZE];
	for(uint i = 0; i < _CHUNK_SIZE; i++)
	{
		map[i] = 0x0000;
	}
}

LightMap::~LightMap()
{
	delete[] map;
}
