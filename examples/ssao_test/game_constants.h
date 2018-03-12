#define VOLUME_VISIBLE_REGION (VR_X*VR_X*VR_X)

#define VISIBLE_REGION Chunk_Dimension(VR_X,VR_Y,VR_Z)
#define VISIBLE_REGION_RADIUS (VISIBLE_REGION/2)
#define WORLD_CHUNK_DIM Chunk_Dimension(CD_X,CD_Y,CD_Z)

global_variable r32 CAMERA_FOCAL_LENGTH = 500.0f;

debug_global r32 GlobalCameraTheta = 0;

#define PLAYER_MODEL MODELS_PATH"/chr_rain.vox"
