#ifndef LIGHTING_H
#define LIGHTING_H
#include "lib.h"
#include "Defines.h"
#include "LightSolver.h"
#include "LightMap.h"
#include "Chunk.h"
#include "Chunks.h"
#include "Block.h"
#include "Voxel.h"

class Lighting
{
	static Chunks* chunks;
	static LightSolver* solverR;
	static LightSolver* solverG;
	static LightSolver* solverB;
	static LightSolver* solverS;

	public:
		static void Clear();
		static int instalize(Chunks* chunks);
		static void finalize();
		/*static void onWorldLoaded();*/
		static void onBlockSet(int x, int y, int z, int id);
		static void onChunkLoaded(int cx, int cy, int cz);
};


#endif // !LIGHTING_H