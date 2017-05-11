#ifndef GAME_CONSTANTS
#define GAME_CONSTANTS

#define CHUNK_VOL (CHUNK_HEIGHT*CHUNK_WIDTH*CHUNK_DEPTH)

// 6 verticies per face, 6 faces per voxel
#define VERT_PER_VOXEL (6*6)

// 3 floats per vert
#define BYTES_PER_VERT (sizeof(r32)*3)

#define VOXEL_DIAMETER (1.0f)
#define VOXEL_RADIUS (VOXEL_DIAMETER/2.0f)

#define CD_X 16
#define CD_Y 16
#define CD_Z 16

// Visible Region XYZ - Must be > (3,3,3)
#define VR_X 24
#define VR_Y 24
#define VR_Z 24

#define VOLUME_VISIBLE_REGION (VR_X*VR_X*VR_X)

#define WORLD_X V3(1,0,0)
#define WORLD_Y V3(0,1,0)
#define WORLD_Z V3(0,0,1)

#define CHUNK_DIMENSION Chunk_Dimension(CD_X,CD_Y,CD_Z)

#define VISIBLE_REGION Chunk_Dimension(VR_X,VR_Y,VR_Z)

#define NOISE_FREQUENCY 200

#define WORLD_HASH_SIZE VOLUME_VISIBLE_REGION
#define FREELIST_SIZE VOLUME_VISIBLE_REGION
#define WORLD_STORAGE_SIZE Megabytes(1200)

#define WORLD_GRAVITY V3(0.0f, -20.0f, 0.0f)

#define PLAYER_STEP_MAX          1
#define PLAYER_JUMP_STRENGTH     20.0f
#define PLAYER_ACCEL_MULTIPLIER  20.0f

/* #define PLAYER_MODEL MODELS_PATH"/chr_knight.vox" */
/* #define PLAYER_MODEL MODELS_PATH"/ephtracy.vox" */
/* #define PLAYER_MODEL MODELS_PATH"/chr_sword.vox" */
/* #define PLAYER_MODEL MODELS_PATH"/shelf.vox" */
/* #define PLAYER_MODEL MODELS_PATH"/8x8x8.vox" */
/* #define PLAYER_MODEL MODELS_PATH"/alien_bot2.vox" */
/* #define PLAYER_MODEL MODELS_PATH"/chr_rain.vox" */
/* #define PLAYER_MODEL MODELS_PATH"/chr_old.vox" */
#define PLAYER_MODEL MODELS_PATH"/3x3x3.vox"


#define DEBUG_SCROLL_WORLD            1
#define DEBUG_CHUNK_AABB              1
#define DEBUG_CAMERA_FOCUS_ORIGIN     0
#define DEBUG_WORLD_GENERATION        0
#define DEBUG_LOD_RENDER              0
#define DEBUG_DRAW_SHADOW_MAP_TEXTURE 0
#define DEBUG_HIGHLIGHT_VOID_CHUNKS   0
#define DEBUG_DRAW_WORLD_AXIES        1
#define DEBUG_OPTIMIZE_WORLD_GC       1
#define DEBUG_THREAD_COUNT_BIAS       0

#define DEBUG_FONT_SIZE               16

#define DEBUG_FRAMES_TO_RUN            -1


#define SHADOW_MAP_XY 50.0f
#define SHADOW_MAP_Z  100.0f
#define SHADOW_MAP_RESOLUTION 2048
#define DEBUG_TEXTURE_SIZE    512

GLOBAL_VARIABLE r32 CAMERA_FOCAL_LENGTH = 100.0f;
GLOBAL_VARIABLE r32 DEBUG_CAMERA_FOCAL_LENGTH = 50.0f;
#define DEBUG_CAMERA_SCROLL_SPEED  20.0f

// TODO(Jesse): How should we initialize this for realz?
DEBUG_GLOBAL canonical_position CameraInitialP = {7};
#define CAMERA_INITIAL_P CameraInitialP

// NOTE(Jesse): The position storage needs to be as large as the largest model
// that will ever be loaded. Can we load models in chunks as well?
#define POSITION_BIT_WIDTH 5
#define COLOR_BIT_WIDTH 8

#define FINAL_POSITION_BIT (POSITION_BIT_WIDTH*3)
#define FINAL_COLOR_BIT ((FINAL_POSITION_BIT+COLOR_BIT_WIDTH))


DEBUG_GLOBAL u32 DEBUG_NOISE_SEED = 65435;

DEBUG_GLOBAL s32 numFrames = 0;
DEBUG_GLOBAL r64 accumulatedTime = 0;

DEBUG_GLOBAL s32 tris = 0;
DEBUG_GLOBAL s32 VoxelsIndexed = 0;

DEBUG_GLOBAL r32 GlobalLightTheta = 0;

DEBUG_GLOBAL aabb LastFreeSlice(V3(0,0,0), V3(0,0,0));
DEBUG_GLOBAL aabb LastQueuedSlice(V3(0,0,0), V3(0,0,0));

DEBUG_GLOBAL bool UseDebugCamera = False;

#define FACE_COLOR_SIZE 32
#define PALETTE_SIZE 256

#define MAX_CHUNK_EDGES 8

#endif
