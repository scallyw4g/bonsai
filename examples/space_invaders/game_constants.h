
#define VOLUME_VISIBLE_REGION (VR_X*VR_X*VR_X)

#define VISIBLE_REGION Chunk_Dimension(VR_X,VR_Y,VR_Z)
#define VISIBLE_REGION_RADIUS (VISIBLE_REGION/2)

global_variable r32 CAMERA_FOCAL_LENGTH = 1000.0f;

debug_global canonical_position CameraInitialP = { {{0,-250,15}}, {0,0,0} };
#define CAMERA_INITIAL_P CameraInitialP
