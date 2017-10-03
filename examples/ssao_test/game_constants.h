#define CD_X 8
#define CD_Y 8
#define CD_Z 8

// Visible Region XYZ - Must be > (3,3,3)
#define VR_X 8
#define VR_Y 8
#define VR_Z 8

#define VOLUME_VISIBLE_REGION (VR_X*VR_X*VR_X)

#define VISIBLE_REGION Chunk_Dimension(VR_X,VR_Y,VR_Z)
#define VISIBLE_REGION_RADIUS (VISIBLE_REGION/2)
#define WORLD_CHUNK_DIM Chunk_Dimension(CD_X,CD_Y,CD_Z)

GLOBAL_VARIABLE r32 CAMERA_FOCAL_LENGTH = 200.0f;
