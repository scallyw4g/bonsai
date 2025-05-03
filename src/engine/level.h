#define LEVEL_FILE_FORMAT_VERSION_NUMBER (1)

struct level_header
{
  u32 LayerCount;
  u32 EntityCount;

  // World Params
  v3i WorldCenter;
  /* v3i VisibleRegion; */
  visible_region_size VisibleRegionSize;

  camera Camera;

  render_settings RenderSettings;

  char TerrainShapingShader[NameBuf_Len];
  char TerrainDecorationShader[NameBuf_Len];
};
