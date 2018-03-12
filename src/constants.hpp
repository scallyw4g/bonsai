#ifndef GAME_CONSTANTS
#define GAME_CONSTANTS

#include <game_render_constants.h>

#define CHUNK_VOL (CHUNK_HEIGHT*CHUNK_WIDTH*CHUNK_DEPTH)

// 6 verticies per face, 6 faces per voxel
#define VERT_PER_VOXEL (6*6)

// 3 floats per vert
#define BYTES_PER_VERT (sizeof(r32)*3)
#define BYTES_PER_FACE (BYTES_PER_VERT*6)

#define VOXEL_DIAMETER (1.0f)
#define VOXEL_RADIUS (VOXEL_DIAMETER/2.0f)

#define WORLD_X V3(1,0,0)
#define WORLD_Y V3(0,1,0)
#define WORLD_Z V3(0,0,1)

#define MAX_VISIBLE_POINT V3(VR_X*CD_X, VR_Y*CD_Y, VR_Z*CD_Z)

#define WORLD_HASH_SIZE 1024
#define FREELIST_SIZE 1024

#define NOISE_FREQUENCY 80L

// This must match the corresponding define in the SSAO shader
#define DEFAULT_ENTROPY_SEED 3426543

#define WORLD_GRAVITY V3(0.0f, 0.0f, 0.0f)

#define SECONDS_PER_LEVEL 10.0f
#define TITLE_FONT_SIZE  32

#define PLAYER_COUNT             1
#define PLAYER_MAX_HP            1000
#define PLAYER_DRAG              V3(0.9f, 0.9f, 0.0f)

/* #define PLAYER_MODEL MODELS_PATH"/chr_knight.vox" */
/* #define PLAYER_MODEL MODELS_PATH"/ephtracy.vox" */
/* #define PLAYER_MODEL MODELS_PATH"/chr_sword.vox" */
/* #define PLAYER_MODEL MODELS_PATH"/shelf.vox" */
/* #define PLAYER_MODEL MODELS_PATH"/8x8x8.vox" */
/* #define PLAYER_MODEL MODELS_PATH"/alien_bot2.vox" */
/* #define PLAYER_MODEL MODELS_PATH"/chr_rain.vox" */
/* #define PLAYER_MODEL MODELS_PATH"/chr_old.vox" */
/* #define PLAYER_MODEL MODELS_PATH"/3x3x3.vox" */
/* #define PLAYER_MODEL MODELS_PATH"/x-wing.vox" */

#define ENEMY_MODEL MODELS_PATH"/tie-fighter.vox"
#define LOOT_MODEL MODELS_PATH"/3x3x3.vox"
#define PROJECTILE_MODEL MODELS_PATH"/3x3x3.vox"

#define TOTAL_ENTITY_COUNT     1024

#define TOTAL_FOLIE_COUNT      64
#define TOTAL_MODEL_COUNT      8
#define TOTAL_PARTICLE_SYSTEMS 32
#define TOTAL_FRAME_EVENT_COUNT 960

#define PARTICLES_PER_SYSTEM   1024

#define ENEMY_SPEED 800
#define PLAYER_SPEED 13000
#define PROJECTILE_SPEED 1500

#define ENEMY_DRAG 0

#define LOOT_CHANCE 1

#define PROJECTILE_DRAG 0
#define PROJECTILE_AABB Chunk_Dimension(1,3,1)

#define DEBUG_ENTITY_DIM     Chunk_Dimension(3,3,1)
#define DEBUG_ENTITY_COLLISION_VOL_RADIUS V3(3.0f, 3.0f, 0.5f)

#define DEBUG_CHUNK_AABB              0
#define DEBUG_DRAW_COLLISION_VOLUMES  0
#define DEBUG_WORLD_GENERATION        0
#define DEBUG_DRAW_SHADOW_MAP_TEXTURE 1
#define DEBUG_HIGHLIGHT_VOID_CHUNKS   0
#define DEBUG_DRAW_WORLD_AXIES        0
#define DEBUG_THREAD_COUNT_BIAS       -2
#define DEBUG_PARTICLE_EFFECTS        0

#define DEBUG_FONT_SIZE               18
#define DEFAULT_LINE_THICKNESS       0.5f

#define DEBUG_FRAMES_TO_RUN           -1

#define DEBUG_TEXTURE_SIZE 500

global_variable r32 DEBUG_CAMERA_FOCAL_LENGTH = 50.0f;
#define DEBUG_CAMERA_SCROLL_SPEED  20.0f

// NOTE(Jesse): The position storage needs to be as large as the largest model
// that will ever be loaded. Can we load models in chunks as well?
#define POSITION_BIT_WIDTH 5
#define COLOR_BIT_WIDTH 8

#define FINAL_POSITION_BIT (POSITION_BIT_WIDTH*3)
#define FINAL_COLOR_BIT ((FINAL_POSITION_BIT+COLOR_BIT_WIDTH))

debug_global u32 DEBUG_NOISE_SEED = 65435;

debug_global s32 numFrames = 0;
debug_global r64 accumulatedTime = 0;

debug_global s32 tris = 0;
debug_global s32 VoxelsIndexed = 0;


#define FACE_VERT_COUNT 6

#define MAX_CHUNK_EDGES 8
#define MAX_LIGHTS 32

#define MIN_LOD_DISTANCE 300

#endif
