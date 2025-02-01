#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdlib.h>
#include "perlin.h"
#include "texture.h"

typedef struct 
{
    vec3 pos;
    enum textureID textId;
} block;

block* blocks;
int numOfBlocks = 0;
void createBlock(vec3 position, enum textureID texId) {
    glm_vec3_copy(position, blocks[numOfBlocks].pos);
    blocks[numOfBlocks].textId = texId;
    numOfBlocks++;
}

void createChunk(int size, int seed) {
    createPerlinNoise(64, 256, 256, seed);
    int width, height, nrChannels;
    unsigned char* heightmapData = stbi_load("perlin.bmp", &width, &height, &nrChannels, 1); // Load as grayscale
    if (!heightmapData) {
        fprintf(stderr, "Failed to load heightmap image.\n");
        return;
    }
    for(int z = 0; z < size; z++) {
		for(int x = 0; x < size; x++) {
            unsigned char pixelValue = heightmapData[z * width + x];
            float normalizedHeight = pixelValue / 255.0f; // Normalize [0, 1]
            int cubeHeight = (int)(normalizedHeight * 12);
            for (int y = 0; y < cubeHeight; ++y) {
                createBlock((vec3){x, y, z}, DIRT);
            }
		}
	}
    stbi_image_free(heightmapData);
}