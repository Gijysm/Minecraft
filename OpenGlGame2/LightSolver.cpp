#include "LightSolver.h"

LightSolver::LightSolver(Chunks* chunks, int channel) : chunks(chunks), channel(channel)
{
}

void LightSolver::Add(int x, int y, int z, int emmision)
{
    if (emmision <= 1)
    {
        return;
    }
    Lightentry entry;
    entry.x = x;
    entry.y = y;
    entry.z = z;
    entry.light = emmision;
    addQueue.push(entry);

    Chunk* chunk = chunks->GetChunkByVoxel(entry.x, entry.y, entry.z);
    if (chunk != nullptr)
    {
        chunk->modifier = true;
        chunk->lightmap->Set(entry.x - chunk->x * _CHUNK_W, entry.y - chunk->y * _CHUNK_H, entry.z - chunk->z * _CHUNK_D, entry.light, channel);
    }
}

void LightSolver::Add(int x, int y, int z)
{
    Add(x, y, z, chunks->getLight(x, y, z, channel));
}

void LightSolver::remove(int x, int y, int z)
{
    Chunk* chunk = chunks->GetChunkByVoxel(x, y, z);
    if (chunk == nullptr)
    {
        return;
    }

    int light = chunk->lightmap->get(x - chunk->x * _CHUNK_W, y - chunk->y * _CHUNK_H, z - chunk->z * _CHUNK_D, channel);
    if (light == 0)
    {
        return;
    }

    Lightentry entry;
    entry.x = x;
    entry.y = y;
    entry.z = z;
    entry.light = light;
    removeQueue.push(entry);

    chunk->lightmap->Set(x - chunk->x * _CHUNK_W, y - chunk->y * _CHUNK_H, z - chunk->z * _CHUNK_D, 0, channel);
}

void LightSolver::solve()
{
    const int coords[] =
    {
        0, 0, 1,
        0, 0, -1,
        0, 1, 0,
        0, -1, 0,
        1, 0, 0,
        -1, 0, 0
    };

    while (!removeQueue.empty())
    {
        Lightentry entry = removeQueue.front();
        removeQueue.pop();

        for (size_t i = 0; i < 6; i++)
        {
            int x = entry.x + coords[i * 3 + 0];
            int y = entry.y + coords[i * 3 + 1];
            int z = entry.z + coords[i * 3 + 2];
            Chunk* chunk = chunks->GetChunkByVoxel(x, y, z);
            if (chunk != nullptr)
            {
                int light = chunks->getLight(x, y, z, channel);
                if (light != 0 && light == entry.light - 1)
                {
                    Lightentry nentry;
                    nentry.x = x;
                    nentry.y = y;
                    nentry.z = z;
                    nentry.light = light;
                    removeQueue.push(nentry);
                    chunk->lightmap->Set(x - chunk->x * _CHUNK_W, y - chunk->y * _CHUNK_H, z - chunk->z * _CHUNK_D, 0, channel);
                    chunk->modifier = true;
                }
                else if (light >= entry.light)
                {
                    Lightentry nentry;
                    nentry.x = x;
                    nentry.y = y;
                    nentry.z = z;
                    nentry.light = light;
                    addQueue.push(nentry);
                }
            }
        }
    }

    while (!addQueue.empty())
    {
        Lightentry entry = addQueue.front();
        addQueue.pop();
        if (entry.light <= 1)
        {
            continue;
        }
        for (size_t i = 0; i < 6; i++)
        {
            int x = entry.x + coords[i * 3 + 0];
            int y = entry.y + coords[i * 3 + 1];
            int z = entry.z + coords[i * 3 + 2];

            Chunk* chunk = chunks->GetChunkByVoxel(x, y, z);
            if (chunk != nullptr)
            {
                int light = chunks->getLight(x, y, z, channel);
                voxel* v = chunks->Get(x, y, z);
                if (v != nullptr && v->id == 0 && light + 2 <= entry.light)
                {
                    chunk->lightmap->Set(x - chunk->x * _CHUNK_W, y - chunk->y * _CHUNK_H, z - chunk->z * _CHUNK_D, entry.light - 1, channel);
                    chunk->modifier = true;
                    Lightentry nentry;
                    nentry.x = x;
                    nentry.y = y;
                    nentry.z = z;
                    nentry.light = entry.light - 1;
                    addQueue.push(nentry);
                }
            }
        }
    }
}
