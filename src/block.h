#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdlib.h>
#include "perlin.h"
#include "texture.h"
#include <time.h>

float vertices[] = {
    // positions          // normals             // texture coords
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f, // Bottom-left
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 0.0f, // Bottom-right
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f, // Top-right
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f, // Top-right
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 1.0f, // Top-left
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f, // Bottom-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f, 1.0f, // Top-right
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f, 0.0f, // Bottom-right
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f, 1.0f, // Top-right
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.0f, 0.0f, // Bottom-left
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.0f, 1.0f, // Top-left
    // Left face
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // Top-right
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 1.0f, // Bottom-left
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, // Top-left
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 1.0f, // Bottom-left
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // Top-right
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 0.0f, // Bottom-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // Top-left
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f, // Top-right
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f, // Bottom-right
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f, // Bottom-right
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // Top-left
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f, // Top-right
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f, // Bottom-left
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 1.0f, // Top-left
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f, // Bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f, // Top-right
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 0.0f, // Bottom-right
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f, // Top-left
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 1.0f, // Top-right
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f, // Bottom-right
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f, // Bottom-right
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 0.0f, // Bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f  // Top-left
};

typedef struct 
{
    vec3 pos;
    enum textureID textId;
    bool visible;
} block;


int numOfBlocks = 0;
void createBlock(vec3 position, enum textureID texId, block* blocks) {
    glm_vec3_copy(position, blocks[numOfBlocks].pos);
    blocks[numOfBlocks].textId = texId;
    numOfBlocks++;
}

block* createChunk(int size, int seed) {
    // Adjusted noise parameters for broader, less frequent mountains
    block* blocks = malloc(size * size * size * sizeof(block));
    createPerlinNoise(64.0f, size, size, time(NULL), 8, 0.2f, 0.9f);
    
    int width, height, nrChannels;
    unsigned char* heightmapData = stbi_load("perlin.bmp", &width, &height, &nrChannels, 1);
    
    if (!heightmapData) {
        fprintf(stderr, "Failed to load heightmap image.\n");
        return NULL;
    }
    
    for(int z = 0; z < size; z++) {
        for(int x = 0; x < size; x++) {
            float normalizedHeight = heightmapData[z * width + x] / 255.0f;
            int cubeHeight;
            cubeHeight = pow(normalizedHeight, 4) * 100;
            for(int y = 0; y < cubeHeight; y++) {
                createBlock((vec3){x, y, z}, STONE, blocks);
            }
        }
    }
    
    stbi_image_free(heightmapData);
    return blocks;
}