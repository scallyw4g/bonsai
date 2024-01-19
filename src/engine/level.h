#define LEVEL_HEADER_VERSION_NUMBER (0x6969696942042042)

struct level_header
{
  u64 Version;
  u32 ChunkCount;
  u32 EntityCount;

  // World Params
  u32 WorldFlags; // world_flag
  v3i WorldCenter;
  v3i VisibleRegion;

  camera Camera;
  cp CameraTarget;
};
