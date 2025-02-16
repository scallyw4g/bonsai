#define LEVEL_FILE_FORMAT_VERSION_NUMBER (1)

struct level_header
{
  u32 EntityCount;

  // World Params
  u32 WorldFlags; // world_flag
  v3i WorldCenter;
  v3i VisibleRegion;

  camera Camera;

  render_settings RenderSettings;
};
