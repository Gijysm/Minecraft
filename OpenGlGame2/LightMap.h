#ifndef LIGHTMAP_H
#define LIGHTMAP_H
#include "lib.h"
#include "Defines.h"
#include "Chunk.h"
class LightMap
{
	public:
	unsigned short* map;
	LightMap();
	~LightMap();
	inline unsigned char get(int x, int y, int z, int channel)
	{
		return (map[y * _CHUNK_D * _CHUNK_W + z * _CHUNK_W + x] >> (channel << 2)) & 0xF;
	}
	inline unsigned char getR(int x, int y, int z)
	{
		return map[y * _CHUNK_D * _CHUNK_W + z * _CHUNK_W + x] & 0xF;
	}
	inline unsigned char getG(int x, int y, int z)
	{
		return (map[y * _CHUNK_D * _CHUNK_W + z * _CHUNK_W + x]>> 4) & 0xF;
	}
	inline unsigned char getB(int x, int y, int z)
	{
		return (map[y * _CHUNK_D * _CHUNK_W + z * _CHUNK_W + x] >> 8) & 0xF;
	}
	inline unsigned char getS(int x, int y, int z)
	{
		return (map[y * _CHUNK_D * _CHUNK_W + z * _CHUNK_W + x] >> 12) & 0xF;
	}	
			 
	inline void SetR(int x, int y, int z, int value)
	{					 
		const int index = y * _CHUNK_D * _CHUNK_W + z * _CHUNK_W + x;
		map[index] = (map[index] & 0xFFF0) | value;
	}					 
	inline void SetG(int x, int y, int z, int value)
	{					 
		const int index = y * _CHUNK_D * _CHUNK_W + z * _CHUNK_W + x;
		map[index] = (map[index] & 0xFFF0) | (value << 4);
	}					 
	inline void SetB(int x, int y, int z, int value)
	{					 
		const int index = y * _CHUNK_D * _CHUNK_W + z * _CHUNK_W + x;
		map[index] = (map[index] & 0xFFF0) | (value << 8);
	}					 
	inline void SetS(int x, int y, int z, int value)
	{					 
		const int index = y * _CHUNK_D * _CHUNK_W + z * _CHUNK_W + x;
		map[index] = (map[index] & 0xFFF0) | (value << 12);
	}

};

#endif