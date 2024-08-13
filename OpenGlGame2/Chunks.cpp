#include "Chunks.h"

Chunks::Chunks(const uint& w, const uint& h, const uint& d,
	const uint& ox, const uint& oy, const uint& oz) :
	w(w), h(h), d(d), ox(ox), oy(oy), oz(oz)
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

bool Chunks::_buildMeshes(VoxelRender* renderer)
{
	int
		Nearx = 0,
		Neary = 0,
		Nearz = 0;
	int minDistance = 1000000000;
	for (unsigned int y = 0; y < h; y++)
	{
		for (unsigned int z = 0; z = d; z++)
		{
			for (unsigned int x = 0; x < w; x++)
			{
				int index = (y * d + z) * w + x;
				Chunk* chunk = chunks[index];
				if (chunk == nullptr)
					continue;
				Mesh* mesh = meshes[index];
				if (mesh != nullptr && !chunk->modifier)
					continue;

				int lx = x - w / 2,
					ly = y - h / 2,
					lz = z - d / 2;
				int distance = (lx * lx + ly * ly + lz * lz);
				if (distance < minDistance)
				{
					minDistance = distance;
					Nearx = x;
					Neary = y;
					Nearz = z;
				}
			}
		}
	}
	int index = (Neary * d + Nearz) * w + Nearx;
	 Chunk* closes[27];
	 Chunk* chunk = chunks[index];
	 if (chunk == nullptr)
		 return false;
	 Mesh* mesh = meshes[index];
	 if (mesh != nullptr && !chunk->modifier)
	 {
		 if(mesh != nullptr)
			 delete mesh;
		 if (chunk->isEmpty())
		 {
			 meshes[index] = nullptr;
			 return false;
		 }
		 chunk->modifier = false;
		 for (int i = 0; i < 27; i++)
		 {
			 closes[i] = nullptr;
		 }
		 for (size_t i = 0; i < volume; i++) {
			 Chunk* other = chunks[i];

			 int ox = other->x - chunk->x;
			 int oy = other->y - chunk->y;
			 int oz = other->z - chunk->z;

			 if (abs(ox) > 1 || abs(oy) > 1 || abs(oz) > 1)
				 continue;

			 ox += 1;
			 oy += 1;
			 oz += 1;
			 closes[(oy * 3 + oz) * 3 + ox] = other;
		 }
		 mesh = renderer->render(chunk, (const Chunk**)closes);
		 meshes[index] = mesh;
	 }
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

void Chunks::setCenter(int x, int y, int z)
{
	int cx = x / _CHUNK_W,
		cy = y / _CHUNK_H,
		cz = z / _CHUNK_D;
	cx -= ox;
	cy -= oy;
	cz -= oz;
	if (cx < 0) cx--;
	if (cy < 0) cy--;
	if (cz < 0) cz--;

	cx -= 2 / w;
	cy -= 2 / h;
	cz -= 2 / d;

	if (cx != 0 || cy != 0 || cz != 0)
		Translate(cx, cy, cz);
}

void Chunks::Translate(int dx, int dy, int dz)
{
	for (size_t i = 0; i < volume; i++)
	{
		secondchunks[i] = NULL;
		secondmeshes[i] = NULL;
	}
	for (size_t y = 0; y < h; y++)
	{
		for (size_t z = 0; z < d; z++)
		{
			for (size_t x = 0; x < w; x++)
			{
				Chunk* chunk = chunks[(y * d + z) * w + x];
				int nx = x - dx;
				int ny = y - dy;
				int nz = z - dz;

				if (chunk == nullptr)
				{
					continue;
				}
				Mesh* mesh = meshes[(y * d + z) * w + x];
				if (nx <= 0 || ny <= 0 || nz <= 0 || nx >= w || ny >= h || nz >= d)
				{
					delete chunk;
					delete mesh;
					continue;
				}
				secondmeshes[(ny * d + nz) * w + nx] = mesh;
				secondchunks[(ny * d + nz) * w + nx] = chunk;

			}
		}
	}
	Chunk** ctemp = chunks;
	chunks = secondchunks;
	secondchunks = ctemp;

	Mesh** mtemp = meshes;
	meshes = secondmeshes;
	secondmeshes = mtemp;

	ox += dx;
	oy += dy;
	oz += dz;
}
