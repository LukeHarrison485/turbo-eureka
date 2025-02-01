#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdlib.h>

typedef enum {
    DIRT,
    GRASS,
    STONE
} textureID;

typedef struct 
{
    vec3 pos;
    textureID textId;
} block;

block* blocks = malloc();