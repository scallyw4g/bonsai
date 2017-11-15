#define CD_X 8
#define CD_Y 32
#define CD_Z 1

// Visible Region XYZ - Must be > (3,3,3)
#define VR_X 12
#define VR_Y 64
#define VR_Z 1

#define VOLUME_VISIBLE_REGION (VR_X*VR_X*VR_X)

#define VISIBLE_REGION Chunk_Dimension(VR_X,VR_Y,VR_Z)
#define VISIBLE_REGION_RADIUS (VISIBLE_REGION/2)
#define WORLD_CHUNK_DIM Chunk_Dimension(CD_X,CD_Y,CD_Z)

GLOBAL_VARIABLE r32 CAMERA_FOCAL_LENGTH = 500.0f;

DEBUG_GLOBAL canonical_position CameraInitialP = { {0,-150,15}, {0,0,0} };
#define CAMERA_INITIAL_P CameraInitialP
