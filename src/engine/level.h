struct level_header
{
  u64 MagicNumber = LEVEL_HEADER_MAGIC_NUMBER; 
  u32 ChunkCount;
  u32 EntityCount;

  // World Params
  u32 WorldFlags; // world_flag
  v3i WorldCenter;
  v3i VisibleRegion;

  camera Camera;
  cp CameraTarget;
};
