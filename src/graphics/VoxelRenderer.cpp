#include "VoxelRenderer.h"
#include "Mesh.h"
#include "../voxels/Chunk.h"
#include "../voxels/voxel.h"

#define VERTEX_SIZE (3 + 2 + 1)

#define IS_IN(X,Y,Z) ((X) >= 0 && (X) < CHUNK_W && (Y) >= 0 && (Y) < CHUNK_H && (Z) >= 0 && (Z) < CHUNK_D)
#define VOXEL(X,Y,Z) (chunk->voxels[((Y) * CHUNK_D + (Z)) * CHUNK_W + (X)])
#define IS_BLOCKED(X,Y,Z) ((IS_IN(X, Y, Z)) && VOXEL(X, Y, Z).id)

#define VERTEX(INDEX, X,Y,Z, U,V, L) buffer[INDEX+0] = (X);\
								  buffer[INDEX+1] = (Y);\
								  buffer[INDEX+2] = (Z);\
								  buffer[INDEX+3] = (U);\
								  buffer[INDEX+4] = (V);\
								  buffer[INDEX+5] = (L);\
								  INDEX += VERTEX_SIZE;

int chunk_attrs[] = {3,2,1, 0};

VoxelRenderer::VoxelRenderer(size_t capacity) : capacity(capacity) {
	buffer = new float[capacity * VERTEX_SIZE * 6];
}

VoxelRenderer::~VoxelRenderer(){
	delete[] buffer;
}

Mesh* VoxelRenderer::render(Chunk* chunk){
	size_t index = 0;
	for (int y = 0; y < CHUNK_H; y++){
		for (int z = 0; z < CHUNK_D; z++){
			for (int x = 0; x < CHUNK_W; x++){
				voxel vox = chunk->voxels[(y * CHUNK_D + z) * CHUNK_W + x];
				unsigned int id = vox.id;

				if (!id){
					continue;
				}

				float l;
				float uvsize = 1.0f/16.0f;
				float u = (id % 16) * uvsize;
				float v = 1-((1 + id / 16) * uvsize);

				if (!IS_BLOCKED(x,y+1,z)){
					l = 1.0f;
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u,v+uvsize, l);

					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u,v+uvsize, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u,v, l);
				}
				if (!IS_BLOCKED(x,y-1,z)){
					l = 0.75f;
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u,v, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u,v+uvsize, l);

					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u,v, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
				}

				if (!IS_BLOCKED(x+1,y,z)){
					l = 0.95f;
					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u+uvsize,v+uvsize, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u,v+uvsize, l);

					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u,v+uvsize, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u,v, l);
				}
				if (!IS_BLOCKED(x-1,y,z)){
					l = 0.85f;
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u,v, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u,v+uvsize, l);

					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u,v, l);
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u+uvsize,v, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
				}

				if (!IS_BLOCKED(x,y,z+1)){
					l = 0.9f;
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u,v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u,v+uvsize, l);

					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u,v, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u+uvsize,v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
				}
				if (!IS_BLOCKED(x,y,z-1)){
					l = 0.8f;
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u+uvsize,v+uvsize, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u,v+uvsize, l);

					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u+uvsize,0, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u,v+uvsize, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u,v, l);
				}
			}
		}
	}
	return new Mesh(buffer, index / VERTEX_SIZE, chunk_attrs);
}
