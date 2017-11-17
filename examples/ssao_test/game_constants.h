#define VOLUME_VISIBLE_REGION (VR_X*VR_X*VR_X)

#define VISIBLE_REGION Chunk_Dimension(VR_X,VR_Y,VR_Z)
#define VISIBLE_REGION_RADIUS (VISIBLE_REGION/2)
#define WORLD_CHUNK_DIM Chunk_Dimension(CD_X,CD_Y,CD_Z)

GLOBAL_VARIABLE r32 CAMERA_FOCAL_LENGTH = 200.0f;

DEBUG_GLOBAL canonical_position CameraInitialP = { {5,-5,15}, {0,0,0} };
#define CAMERA_INITIAL_P CameraInitialP

DEBUG_GLOBAL r32 GlobalCameraTheta = 0;
