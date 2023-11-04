#define LEVEL_HEADER_MAGIC_NUMBER (0x6969696942042042)


#pragma pack(push, 1)
struct level_header
{
  u64 MagicNumber = LEVEL_HEADER_MAGIC_NUMBER; 
  u32 ChunkCount;

  // World Params
  u32 WorldFlags; // world_flag
  v3i WorldCenter;
  v3i VisibleRegion;

  camera Camera;
  cp CameraTarget;
};
#pragma pack(pop)
