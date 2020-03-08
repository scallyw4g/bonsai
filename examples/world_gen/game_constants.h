// Visible Region XYZ - Must be > (3,3,3)
#define VR_X 32
#define VR_Y 32
#define VR_Z 16

// Texture Resolution
#define SHADOW_MAP_RESOLUTION_X (4096)
#define SHADOW_MAP_RESOLUTION_Y (4096)

// World-Space draw resolution
#define SHADOW_MAP_X (50)
#define SHADOW_MAP_Y (50)

#define SHADOW_MAP_Z_MIN (-100.0f)
#define SHADOW_MAP_Z_MAX ( 100.0f)


global_variable r32 CAMERA_FOCAL_LENGTH = 150.0f;

debug_global r32 GlobalCameraTheta = 0;

#define PLAYER_MODEL MODELS_PATH"/chr_old.vox"
