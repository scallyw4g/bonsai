#ifndef GAME_CONSTANTS
#define GAME_CONSTANTS

#define True 0
#define False 1

#define DEBUG_GLOBAL static

#define BONSAI_INTERNAL 1

#define CD_X 8
#define CD_Y 8
#define CD_Z 8

// Visible Region XYZ - Must be > (3,3,3)
#define VR_X 8
#define VR_Y 8
#define VR_Z 8

#define WORLD_X V3(1,0,0)
#define WORLD_Y V3(0,1,0)
#define WORLD_Z V3(0,0,1)

#define CHUNK_DIMENSION Chunk_Dimension(CD_X,CD_Y,CD_Z)

#define VISIBLE_REGION Chunk_Dimension(VR_X,VR_Y,VR_Z)

#define WORLD_SIZE 200

#define DEBUG_SCROLL_WORLD         1
#define DEBUG_CHUNK_AABB           0
#define DEBUG_CAMERA_FOCUS_ORIGIN  0
#define DEBUG_OPTIMIZE_TRI_COUNT   1
#define DEBUG_WORLD_GENERATION     0
#define DEBUG_LOD_RENDER           0

#define DEBUG_FRAMES_TO_RUN       -1

#define PLAYER_STEP_MAX            1

#define CHUNK_STACK_SIZE (VR_X*VR_X*VR_X)

#define CAMERA_FOCAL_LENGTH 80.0f;

#define CAMERA_INITIAL_P Canonical_Position(V3(1,1,1), World_Position(world.VisibleRegion/2))

#define POSITION_BIT_WIDTH 6
#define COLOR_BIT_WIDTH 8

#define FINAL_POSITION_BIT (POSITION_BIT_WIDTH*3)
#define FINAL_COLOR_BIT ((FINAL_POSITION_BIT+COLOR_BIT_WIDTH))

DEBUG_GLOBAL float PLAYER_ACCEL_MULTIPLIER = 5.0f;

DEBUG_GLOBAL int numFrames = 0;
DEBUG_GLOBAL float accumulatedTime = 0;

DEBUG_GLOBAL int tris = 0;
DEBUG_GLOBAL int VoxelsIndexed = 0;

#define CHUNK_VOL (CHUNK_HEIGHT*CHUNK_WIDTH*CHUNK_DEPTH)

// 6 verticies per face, 6 faces per voxel
#define VERT_PER_VOXEL (6*6)

#define VOXEL_DIAMETER 1.0f
#define VOXEL_RADIUS (VOXEL_DIAMETER/2.0f)

#define FACE_COLOR_SIZE 32
#define PALETTE_SIZE 256

#endif
