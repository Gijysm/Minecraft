#include "Chunks.h"

Chunks::Chunks(uint w, uint h, uint d) :w(w), h(h), d(d)
{
	volume = w * h * d;
	this->chunks = new Chunk*[volume];
	int index = 0;

	for (int y = 0; y < h; y++) {
		for (int z = 0; z < d; z++) {
			for (int x = 0; x < w; x++) {
				Chunk* chunk = new Chunk(x,y,z);
				chunks[index] = chunk;
				index++;
			}
		}
	}
}

const size_t& Chunks::Getvolume()
{
	return volume;
}

Chunk* Chunks::GetChunk(int i)
{
	if (i < 0 || i >= Getvolume()) {
		return nullptr;  // Або кидайте виключення.
	}
	return chunks[i];
}

Chunks::~Chunks()
{
	for (int i = 0; i < volume; i++) {
		delete chunks[i];
	}
	delete[] chunks;
}
