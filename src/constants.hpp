#ifndef GAME_CONSTANTS
#define GAME_CONSTANTS

#define CD_X 8
#define CD_Y 8
#define CD_Z 8

// Visible Region XYZ - Must be > (3,3,3)
#define VR_X 8
#define VR_Y 8
#define VR_Z 8

#define CHUNK_DIMENSION Chunk_Dimension(CD_X,CD_Y,CD_Z)

#define VISIBLE_REGION Chunk_Dimension(VR_X,VR_Y,VR_Z)


#define WORLD_SIZE 200

#define DEBUG_SCROLL_WORLD         1
#define DEBUG_CHUNK_AABB           0
#define DEBUG_CAMERA_FOCUS_ORIGIN  0
#define DEBUG_OPTIMIZE_TRI_COUNT   1
#define DEBUG_WORLD_GENERATION     1
#define DEBUG_LOD_RENDER           0

#define DEBUG_FRAMES_TO_RUN       -1

#define PLAYER_STEP_MAX 1


#define CHUNK_STACK_SIZE (VR_X*VR_X*VR_X)

#define CAMERA_OFFSET V3(0,50,55)

static int numFrames = 0;
static float accumulatedTime = 0;

static int tris = 0;
static int VoxelsIndexed = 0;

#define CHUNK_VOL (CHUNK_HEIGHT*CHUNK_WIDTH*CHUNK_DEPTH)

// 6 verticies per face, 6 faces per voxel
#define VERT_PER_VOXEL (6*6)

#define VOXEL_DIAMETER 1.0f
#define VOXEL_RADIUS (VOXEL_DIAMETER/2.0f)

#endif
