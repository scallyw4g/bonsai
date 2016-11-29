#ifndef GAME_CONSTANTS
#define GAME_CONSTANTS

#define CHUNK_WIDTH  32
#define CHUNK_HEIGHT 32
#define CHUNK_DEPTH  32

#define VISIBLE_REGION Chunk_Dimension(6,6,6) // Must be > (3,3,3)

#define PERLIN_NOISE_GENERATION 1

#define WORLD_SIZE 100

#define DEBUG_SCROLL_WORLD 1

#define BOUNDARY_VOXELS_UNINITIALIZED INT_MAX

#define CAMERA_OFFSET V3(0,20,25)

static int numFrames = 0;
static float accumulatedTime = 0;

static int tris = 0;
static int VoxelsIndexed = 0;
static int BoundaryVoxelsIndexed = 0;

static int thing = 0;

#define CHUNK_VOL (CHUNK_HEIGHT*CHUNK_WIDTH*CHUNK_DEPTH)

// 6 verticies per face, 6 faces per voxel
#define VERT_PER_VOXEL (6*6)
// Number of array elements required to render a voxel
#define BYTES_PER_VOXEL (VERT_PER_VOXEL*3*sizeof(GLfloat))

#define VOXEL_DIAMETER 1.0f
#define VOXEL_RADIUS (VOXEL_DIAMETER/2.0f)

#endif
