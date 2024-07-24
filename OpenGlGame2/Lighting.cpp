#include "Lighting.h"

Chunks* Lighting::chunks = nullptr;
LightSolver* Lighting::solverR = nullptr;
LightSolver* Lighting::solverG = nullptr;
LightSolver* Lighting::solverB = nullptr;
LightSolver* Lighting::solverS = nullptr;

int Lighting::instalize(Chunks* chunks)
{
	Lighting::chunks = chunks;
	Lighting::solverR = new LightSolver(chunks, 0);
	Lighting::solverG = new LightSolver(chunks, 1);
	Lighting::solverB = new LightSolver(chunks, 2);
	Lighting::solverS = new LightSolver(chunks, 3);
	return 0;
}

void Lighting::finalize()
{
	delete solverR;
	delete solverG;
	delete solverB;
	delete solverS;
}

void Lighting::onWorldLoaded()
{
    for (size_t y = 0; y < chunks->h * _CHUNK_H; y++)
    {
        for (size_t z = 0; z < chunks->d * _CHUNK_D; z++)
        {
            for (size_t x = 0; x < chunks->w * _CHUNK_W; x++)
            {
                voxel* vox = chunks->Get(x, y, z);
                if (vox->id == 4)
                {
                    solverR->Add(x, y, 15);
                    solverG->Add(x, y, 15);
                    solverB->Add(x, y, 15);
                }
            }
        }
    }
    for (size_t z = 0; z < chunks->d * _CHUNK_D; z++)
    {
        for (size_t x = 0; x < chunks->w * _CHUNK_W; x++)
        {
            for (size_t y = chunks->h * _CHUNK_H - 1; y >= 0; y--)
            {
                voxel* vox = chunks->Get(x, y, z);
                if (vox->id != 0) {
                    break;
                }
                chunks->GetChunkByVoxel(x, y, z)->lightmap->SetS(x % _CHUNK_W, y % _CHUNK_H, z % _CHUNK_D, 0xF);
            }

        }
    }

    for (int z = 0; z < chunks->d * _CHUNK_D; z++) {
        for (int x = 0; x < chunks->w * _CHUNK_W; x++) {
            for (int y = chunks->h * _CHUNK_H - 1; y >= 0; y--) {
                voxel* vox = chunks->Get(x, y, z);
                if (vox->id != 0) {
                    break;
                }
                if (chunks->getLight(x - 1, y, z, 3) == 0 ||
                    chunks->getLight(x + 1, y, z, 3) == 0 ||
                    chunks->getLight(x, y - 1, z, 3) == 0 ||
                    chunks->getLight(x, y + 1, z, 3) == 0 ||
                    chunks->getLight(x, y, z - 1, 3) == 0 ||
                    chunks->getLight(x, y, z + 1, 3) == 0) {
                    solverS->Add(x, y, z);
                }
                Chunk* chunk = chunks->GetChunkByVoxel(x, y, z);
                if (!chunk) {
                    std::cerr << "Null chunk at voxel (" << x << ", " << y << ", " << z << ")" << std::endl;
                    continue;
                }

                if (!chunk->lightmap) {
                    std::cerr << "Null lightmap at chunk (" << chunk->x << ", " << chunk->y << ", " << chunk->z << ")" << std::endl;
                    continue;
                }

                chunk->lightmap->SetS(x % _CHUNK_W, y % _CHUNK_H, z % _CHUNK_D, 0xF);
            }

        }
    }
    solverR->solve();
    solverG->solve();
    solverB->solve();
    solverS->solve();
}

void Lighting::onBlockSet(int x, int y, int z, int id)
{
    if(id == 0)

    {
        solverR->remove(x, y, z);
        solverG->remove(x, y, z);
        solverB->remove(x, y, z);
        solverS->remove(x, y, z);

        solverR->solve();
        solverG->solve();
        solverB->solve();
        if (chunks->getLight(x, y + 1, z, 3) == 0xF)
        {
            for (int i = y; i > 0; i--)
            {
                if (chunks->Get(x, i, z)->id != 0)
                    break;
                solverS->Add(x, i, z, 0xF);
            }
        }
        solverR->Add(x, y + 1, z); solverG->Add(x, y + 1, z); solverB->Add(x, y + 1, z); solverS->Add(x, y + 1, z);
        solverR->Add(x, y - 1, z); solverG->Add(x, y - 1, z); solverB->Add(x, y - 1, z); solverS->Add(x, y - 1, z);
        solverR->Add(x + 1, y, z); solverG->Add(x + 1, y, z); solverB->Add(x + 1, y, z); solverS->Add(x + 1, y, z);
        solverR->Add(x - 1, y, z); solverG->Add(x - 1, y, z); solverB->Add(x - 1, y, z); solverS->Add(x - 1, y, z);
        solverR->Add(x, y, z + 1); solverG->Add(x, y, z + 1); solverB->Add(x, y, z + 1); solverS->Add(x, y, z + 1);
        solverR->Add(x, y, z - 1); solverG->Add(x, y, z - 1); solverB->Add(x, y, z - 1); solverS->Add(x, y, z - 1);

        solverR->solve();
        solverG->solve();
        solverB->solve();
        solverS->solve();
    }
    else
    {
        solverR->Add(x, y, z);
        solverG->Add(x, y, z);
        solverB->Add(x, y, z);
        solverS->Add(x, y, z);
        for (int i = y; i > 0; i--)
        {
            solverS->remove(x, i, z);
            if (i == 0 || chunks->Get(x, i, z)->id != 0)
                break;
        }
        solverR->solve();
        solverG->solve();
        solverB->solve();
        solverS->solve();
        Block* block = Block::blocks[id];
        if (block->emission[0] || block->emission[1] || block->emission[2])
        {
            solverR->Add(x, y, z, block->emission[0]);
            solverG->Add(x, y, z, block->emission[1]);
            solverB->Add(x, y, z, block->emission[2]);
            solverR->solve();
            solverG->solve();
            solverB->solve();
        }

    }
}
