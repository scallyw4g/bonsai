#ifndef GAME_CONSTANTS
#define GAME_CONSTANTS

#define CHUNK_WIDTH  64
#define CHUNK_HEIGHT 64
#define CHUNK_DEPTH  64


#define CHUNK_VOL (CHUNK_HEIGHT*CHUNK_WIDTH*CHUNK_DEPTH)

// 6 verticies per face, 6 faces per voxel
#define VERT_PER_VOXEL (6*6)
// Number of array elements required to render a voxel
#define VERT_DATA_POINTS_PER_VOXEL (VERT_PER_VOXEL*3)

// number of verticies*3 (for each x, y, z)
#define WORLD_VERTEX_COUNT (VERT_DATA_POINTS_PER_VOXEL*CHUNK_VOL)

// length of worldvertexbuffer in bytes
#define WORLD_VERTEX_BUFFER_SIZE (WORLD_VERTEX_COUNT*sizeof(GLfloat))

#define VOXEL_DIAMETER 1.0f
#define VOXEL_RADIUS VOXEL_DIAMETER/2.0f


static GLfloat g_WorldVertexColorData[WORLD_VERTEX_COUNT] = {};

static const GLfloat g_VoxelColorBuffer[VERT_DATA_POINTS_PER_VOXEL] = {
    // Bottom
    0.0f,  1.0f,  1.0f,
    0.0f,  1.0f,  1.0f,
    0.0f,  1.0f,  1.0f,

    0.0f,  1.0f,  1.0f,
    0.0f,  1.0f,  1.0f,
    0.0f,  1.0f,  1.0f,

    // Left
    1.0f,  0.0f,  1.0f,
    1.0f,  0.0f,  1.0f,
    1.0f,  0.0f,  1.0f,

    1.0f,  0.0f,  1.0f,
    1.0f,  0.0f,  1.0f,
    1.0f,  0.0f,  1.0f,

    // Back
    1.0f,  1.0f,  0.0f,
    1.0f,  1.0f,  0.0f,
    1.0f,  1.0f,  0.0f,

    1.0f,  1.0f,  0.0f,
    1.0f,  1.0f,  0.0f,
    1.0f,  1.0f,  0.0f,

    // Front
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,

    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,

    // Right
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,

    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,

    // Top
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,

    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f
  };

#endif
