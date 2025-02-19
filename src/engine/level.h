#define LEVEL_FILE_FORMAT_VERSION_NUMBER (1)

struct level_header
{
  u32 EditCount;
  u32 EntityCount;

  // World Params
  v3i WorldCenter;
  v3i VisibleRegion;

  camera Camera;

  render_settings RenderSettings;

  char TerrainGenShader[NameBuf_Len];
};
