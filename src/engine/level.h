#define LEVEL_FILE_FORMAT_VERSION_NUMBER (1)

struct level_header
{
  u32 LayerCount;
  u32 EditCount;
  u32 EntityCount;

  // Editor Params
  //
  u32 SelectedLayerIndex;

  // World Params
  //
  v3i WorldCenter; // in world chunks ..?
  visible_region_size VisibleRegionSize;

  camera Camera;
  render_settings RenderSettings;

  char TerrainShapingShader[NameBuf_Len];
  char TerrainDecorationShader[NameBuf_Len];
};
