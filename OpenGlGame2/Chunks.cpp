#include "Chunks.h"

Chunks::Chunks(uint w, uint h, uint d) :w(w), h(h), d(d)
{
	volume = w * h * d;
	this->chunks = new Chunk*[volume];
	int index = 0;

	for (int y = 0; y < h; y++) {
		for (int z = 0; z < d; z++) {
			for (int x = 0; x < w; x++, index++) {
				Chunk* chunk = new Chunk(x,y,z);
				chunks[index] = chunk;
			}
		}
	}
}

const size_t& Chunks::Getvolume()
{
	return volume;
}

unsigned char Chunks::getLight(int x, int y, int z, int channel)
{
	int cx = x / _CHUNK_W;
	int cy = y / _CHUNK_H;
	int cz = z / _CHUNK_D;
	if (x < 0) cx--;
	if (y < 0) cy--;
	if (z < 0) cz--;
	if (cx < 0 || cy < 0 || cz < 0 || cx >= w || cy >= h || cz >= d)
		return 0;
	Chunk* chunk = chunks[(cy * d + cz) * w + cx];
	int lx = x - cx * _CHUNK_W;
	int ly = y - cy * _CHUNK_H;
	int lz = z - cz * _CHUNK_D;
	return chunk->lightmap->get(lx, ly, lz, channel);
}

Chunk* Chunks::GetChunk(int i)
{
	if (i < 0 || i >= Getvolume()) {
		return nullptr;  // Або кидайте виключення.
	}
	return chunks[i];
}

Chunk* Chunks::GetChunk(int x, int y, int z)
{
	if (x < 0 || y < 0 || z < 0 || x >= w || y >= h || z >= d) // Виправлена логіка
		return nullptr;
	return chunks[(y * d + z) * w + x];
}

Chunks::~Chunks()
{
	for (int i = 0; i < volume; i++) {
		delete chunks[i];
	}
	delete[] chunks;
}
voxel* Chunks::Get(int x, int y, int z) {
	int cx = x / _CHUNK_W;
	int cy = y / _CHUNK_H;
	int cz = z / _CHUNK_D;
	if (x < 0) cx--;
	if (y < 0) cy--;
	if (z < 0) cz--;
	if (cx < 0 || cy < 0 || cz < 0 || cx >= w || cy >= h || cz >= d)  // Виправлена логіка
		return nullptr;
	Chunk* chunk = chunks[(cy * d + cz) * w + cx];
	int lx = x - cx * _CHUNK_W;
	int ly = y - cy * _CHUNK_H;
	int lz = z - cz * _CHUNK_D;
	return &chunk->voxels[(ly * _CHUNK_D + lz) * _CHUNK_W + lx];
}

voxel* Chunks::rayCast(vec3 a, vec3 dir, float maxDistance, vec3& end, vec3& norm, vec3& iend)
{
	float px = a.x;
	float py = a.y;
	float pz = a.z;

	float dx = dir.x;
	float dy = dir.y;
	float dz = dir.z;

	float t = 0;
	int ix = floor(px);
	int iy = floor(py);
	int iz = floor(pz);

	float stepx = (dx > 0) ? 1.f : -1.f;
	float stepy = (dy > 0) ? 1.f : -1.f;
	float stepz = (dz > 0) ? 1.f : -1.f;
	constexpr float infinity = std::numeric_limits<float>::infinity();

	float txDelta = (dx == 0.0f) ? infinity : abs(1.0f / dx);
	float tyDelta = (dy == 0.0f) ? infinity : abs(1.0f / dy);
	float tzDelta = (dz == 0.0f) ? infinity : abs(1.0f / dz);

	float xdist = (stepx > 0) ? (ix + 1 - px) : (px - ix);
	float ydist = (stepy > 0) ? (iy + 1 - py) : (py - iy);
	float zdist = (stepz > 0) ? (iz + 1 - pz) : (pz - iz);

	float txMax = (txDelta < infinity) ? txDelta * xdist : infinity;
	float tyMax = (tyDelta < infinity) ? tyDelta * ydist : infinity;
	float tzMax = (tzDelta < infinity) ? tzDelta * zdist : infinity;
	int steppedIndex = -1;

	while (t <= maxDistance)
	{
		voxel* voxel = Get(ix, iy, iz);
		if (voxel != nullptr && voxel->id)

		{
			end.x = px + t * dx;
			end.y = py + t * dy;
			end.z = pz + t * dz;

			iend.x = ix;
			iend.y = iy;
			iend.z = iz;
			norm.x = norm.y = norm.z = 0;
			if (steppedIndex == 0) norm.x = -stepx;
			if (steppedIndex == 1) norm.y = -stepy;
			if (steppedIndex == 2) norm.z = -stepz;
			return voxel;
		}
		if (txMax < tyMax)
		{
			if (txMax < tzMax)
			{
				ix += stepx;
				t = txMax;
				txMax += txDelta;
				steppedIndex = 0;
			}
			else
			{
				iz += stepz;
				t = tzMax;
				tzMax += tzDelta;
				steppedIndex = 2;
			}
		}
		else
		{
			if (tyMax < tzMax)
			{
				iy += stepy;
				t = tyMax;
				tyMax += tyDelta;
				steppedIndex = 1;
			}
			else
			{
				iz += stepz;
				t = tzMax;
				tzMax += tzDelta;
				steppedIndex = 2;
			}
		}
	}
	iend.x = ix;
	iend.y = iy;
	iend.z = iz;

	end.x = px + t * dx;
	end.y = py + t * dy;
	end.z = pz + t * dz;
	norm.x = norm.y = norm.z = 0.0f;
	return nullptr;
}

void Chunks::Set(int x, int y, int z, int id) {
	int cx = x / _CHUNK_W;
	int cy = y / _CHUNK_H;
	int cz = z / _CHUNK_D;
	if (x < 0) cx--;
	if (y < 0) cy--;
	if (z < 0) cz--;
	if (cx < 0 || cy < 0 || cz < 0 || cx >= w || cy >= h || cz >= d)  // Виправлена логіка
		return;
	Chunk* chunk = chunks[(cy * d + cz) * w + cx];
	int lx = x - cx * _CHUNK_W;
	int ly = y - cy * _CHUNK_H;
	int lz = z - cz * _CHUNK_D;
	chunk->voxels[(ly * _CHUNK_D + lz) * _CHUNK_W + lx].id = id;
	chunk->modifier = true;

	if (lx == 0 && (chunk = GetChunk(cx - 1, cy, cz))) chunk->modifier = true;
	if (ly == 0 && (chunk = GetChunk(cx, cy - 1, cz))) chunk->modifier = true;
	if (lz == 0 && (chunk = GetChunk(cx, cy, cz - 1))) chunk->modifier = true;

	if (lx == _CHUNK_W - 1 && (chunk = GetChunk(cx + 1, cy, cz))) chunk->modifier = true;
	if (ly == _CHUNK_H - 1 && (chunk = GetChunk(cx, cy + 1, cz))) chunk->modifier = true;
	if (lz == _CHUNK_D - 1 && (chunk = GetChunk(cx, cy, cz + 1))) chunk->modifier = true;
}

void Chunks::write(unsigned char* info)
{
	size_t index = 0;
	for (int i = 0; i < volume; i++) {
		Chunk* chunk = chunks[i];

		for (size_t j = 0; j < _CHUNK_SIZE; j++, index++)
		{
			info[index] = chunk->voxels[j].id;
		}
	}
}

void Chunks::read(unsigned char* source)
{
	size_t index = 0;
	for (int i = 0; i < volume; i++) {
		Chunk* chunk = chunks[i];

		for (size_t j = 0; j < _CHUNK_SIZE; j++, index++)
		{
			source[index] = chunk->voxels[j].id;
			chunk->voxels[j].id = source[index];
		}
		chunk->modifier = true; 
	}
}
Chunk* Chunks::GetChunkByVoxel(int x, int y, int z)
{
	int cx = x / _CHUNK_W;
	int cy = y / _CHUNK_H;
	int cz = z / _CHUNK_D;
	if (x < 0) cx--;
	if (y < 0) cy--;
	if (z < 0) cz--;
	if (cx < 0 || cy < 0 || cz < 0 || cx >= w || cy >= h || cz >= d)
		return nullptr;
	return chunks[(cy * d + cz) * w + cx];
}
