#ifndef LIGHTSOLVER_H
#define LIGHTSOLVER_H
#include "lib.h"
#include "LightMap.h"
#include "Defines.h"
#include "Chunks.h"
#include "Chunk.h"
#include "Voxel.h"
struct Lightentry
{
	int x, 
		y,
		z;
	uint light;
};
class LightSolver
{
	queue<Lightentry> addQueue;
	queue<Lightentry> removeQueue;
	Chunks* chunks;
	int channel;
	public:
		LightSolver(Chunks* chunks, int channel);
		void Add(int x, int y, int z, int emmision);
		void Add(int x, int y, int z);
		void remove(int x, int y, int z);
		void solve();
};

#endif // LIGHTSOLVER_H