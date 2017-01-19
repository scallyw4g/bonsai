#ifndef GAME_CONSTANTS
#define GAME_CONSTANTS

#define DEBUG_GLOBAL static

#define CD_X 16
#define CD_Y 16
#define CD_Z 16

// Visible Region XYZ - Must be > (3,3,3)
#define VR_X 4
#define VR_Y 4
#define VR_Z 4

#define WORLD_UP V3(0,1,0)

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

#define CAMERA_INITIAL_P Canonical_Position(V3(0,50,55), World_Position(world.VisibleRegion/2))

#define N_VOXEL_STORAGE_BITS 8

DEBUG_GLOBAL int numFrames = 0;
DEBUG_GLOBAL float accumulatedTime = 0;

DEBUG_GLOBAL int tris = 0;
DEBUG_GLOBAL int VoxelsIndexed = 0;

#define CHUNK_VOL (CHUNK_HEIGHT*CHUNK_WIDTH*CHUNK_DEPTH)

// 6 verticies per face, 6 faces per voxel
#define VERT_PER_VOXEL (6*6)

#define VOXEL_DIAMETER 1.0f
#define VOXEL_RADIUS (VOXEL_DIAMETER/2.0f)

#endif
