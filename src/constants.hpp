#ifndef GAME_CONSTANTS
#define GAME_CONSTANTS

#define CD_X 8
#define CD_Y 8
#define CD_Z 8

// Visible Region XYZ - Must be > (3,3,3)
#define VR_X 32
#define VR_Y 32
#define VR_Z 32

#define CHUNK_DIMENSION Chunk_Dimension(CD_X,CD_Y,CD_Z)

#define VISIBLE_REGION Chunk_Dimension(VR_X,VR_Y,VR_Z)

#define PERLIN_NOISE_GENERATION 1

#define WORLD_SIZE 100

#define DEBUG_SCROLL_WORLD 1

#define BOUNDARY_VOXELS_UNINITIALIZED -1 // THIS MUST BE NEGATIVE

#define CHUNK_STACK_SIZE (VR_X*VR_X*VR_X)

static ChunkStack FreeChunks;

#define CAMERA_OFFSET V3(0,40,45)

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
