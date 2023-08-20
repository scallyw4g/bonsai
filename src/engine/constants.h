// 6 verticies per face, 6 faces per voxel
#define VERTS_PER_FACE (6)
#define VERTS_PER_VOXEL (VERTS_PER_FACE*6)
#define VERTS_PER_PARTICLE VERTS_PER_VOXEL
#define VERTS_PER_LINE (18)


#define WORLD_X V3(1,0,0)
#define WORLD_Y V3(0,1,0)
#define WORLD_Z V3(0,0,1)

// NOTE(Jesse): This should probably be dynamic by now..
#define FREELIST_SIZE (Kilobytes(8))

#define NOISE_FREQUENCY (100L)

#define WORLD_GRAVITY (V3(0.0f, 0.0f, 0.0f))

#define MODELS_PATH "models"

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

#define DEBUG_CHUNK_AABB              0
#define DEBUG_DRAW_COLLISION_VOLUMES  0
#define DEBUG_WORLD_GENERATION        0
#define DEBUG_HIGHLIGHT_VOID_CHUNKS   0
#define DEBUG_DRAW_WORLD_AXIES        1
#define DEBUG_DRAW_FRUSTUM_AT_ORIGIN  0
#define DEBUG_PARTICLE_EFFECTS        1

#define ENEMY_MODEL      MODELS_PATH "/tie-fighter.vox"
#define LOOT_MODEL       MODELS_PATH "/3x3x3.vox"
#define PROJECTILE_MODEL MODELS_PATH "/3x3x3.vox"

#define TOTAL_ENTITY_COUNT     (1024*2)

// THIS MUST MATCH THE DEFINE IN header.glsl
// Also must be a power of two
#define MAX_LIGHTS 1024
