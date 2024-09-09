#define LEVEL_FILE_FORMAT_VERSION_NUMBER (1)

struct level_header poof(@version(4))
{
  u32 ChunkCount;
  u32 EntityCount;

  // World Params
  u32 WorldFlags; // world_flag
  v3i WorldCenter;
  v3i VisibleRegion;

  camera Camera;

  render_settings RenderSettings;
};

// NOTE(Jesse): This version and previous versions had chunks with magicavoxel encoded colors.
struct level_header_3
{
  u32 ChunkCount;
  u32 EntityCount;

  // World Params
  u32 WorldFlags; // world_flag
  v3i WorldCenter;
  v3i VisibleRegion;

  camera Camera;

  render_settings RenderSettings;
};

struct level_header_2
{
  bonsai_type_info_hashtable TypeInfoHashtable; poof(@no_serialize)

  u32 ChunkCount;
  u32 EntityCount;

  // World Params
  u32 WorldFlags; // world_flag
  v3i WorldCenter;
  v3i VisibleRegion;

  camera Camera;
  cp CameraTarget;
};


struct level_header_1
{
  /* u64 Version; */
  bonsai_type_info_hashtable TypeInfoHashtable; poof(@no_serialize)

  u32 ChunkCount;
  u32 EntityCount;

  // World Params
  u32 WorldFlags; // world_flag
  v3i WorldCenter;
  v3i VisibleRegion;

  camera Camera;
  cp CameraTarget;
};


struct level_header_0
{
  /* u64 Version; */
  u32 ChunkCount;
  u32 EntityCount;

  // World Params
  u32 WorldFlags; // world_flag
  v3i WorldCenter;
  v3i VisibleRegion;

  camera Camera;
  cp CameraTarget;
};
