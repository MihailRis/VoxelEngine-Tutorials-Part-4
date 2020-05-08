#include "Chunk.h"
#include "voxel.h"
#include <math.h>

Chunk::Chunk(){
	voxels = new voxel[CHUNK_VOL];
	for (int y = 0; y < CHUNK_H; y++){
		for (int z = 0; z < CHUNK_D; z++){
			for (int x = 0; x < CHUNK_W; x++){
				int id = y <= (sin(x*0.3f) * 0.5f + 0.5f) * 10;
				if (y <= 2)
					id = 2;
				voxels[(y * CHUNK_D + z) * CHUNK_W + x].id = id;
			}
		}
	}
}

Chunk::~Chunk(){
	delete[] voxels;
}
