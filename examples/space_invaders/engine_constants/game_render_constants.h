#define CD_X 8
#define CD_Y 32
#define CD_Z 1

// Visible Region XYZ - Must be > (3,3,3)
#define VR_X 12
#define VR_Y 64
#define VR_Z 1


#define WORLD_CHUNK_DIM Chunk_Dimension(CD_X,CD_Y,CD_Z)
GLOBAL_VARIABLE chunk_dimension Global_WorldChunkDim = WORLD_CHUNK_DIM;

// Texture Resolution
#define SHADOW_MAP_RESOLUTION_X 512
#define SHADOW_MAP_RESOLUTION_Y 4096

// World-Space draw resolution
#define SHADOW_MAP_X (100)
#define SHADOW_MAP_Y (1400)

#define SHADOW_MAP_Z_MIN  -100.0f
#define SHADOW_MAP_Z_MAX  200.0f

