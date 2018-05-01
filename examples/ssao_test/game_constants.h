#define CD_X 16
#define CD_Y 16
#define CD_Z 8

// Visible Region XYZ - Must be > (3,3,3)
#define VR_X 16
#define VR_Y 16
#define VR_Z 3


#define WORLD_CHUNK_DIM Chunk_Dimension(CD_X,CD_Y,CD_Z)

// Texture Resolution
#define SHADOW_MAP_RESOLUTION_X (4096)
#define SHADOW_MAP_RESOLUTION_Y (4096)

// World-Space draw resolution
#define SHADOW_MAP_X (50)
#define SHADOW_MAP_Y (50)

#define SHADOW_MAP_Z_MIN (-100.0f)
#define SHADOW_MAP_Z_MAX ( 100.0f)

#define VOLUME_VISIBLE_REGION (VR_X*VR_X*VR_X)

#define VISIBLE_REGION Chunk_Dimension(VR_X,VR_Y,VR_Z)
#define VISIBLE_REGION_RADIUS (VISIBLE_REGION/2)
#define WORLD_CHUNK_DIM Chunk_Dimension(CD_X,CD_Y,CD_Z)

global_variable r32 CAMERA_FOCAL_LENGTH = 150.0f;

debug_global r32 GlobalCameraTheta = 0;

#define PLAYER_MODEL MODELS_PATH"/chr_old.vox"
