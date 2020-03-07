
struct render_state
{
  window_layout*  Window;
  u32 WindowStartCommandIndex;

  layout* Layout;

  b32 Hover;
  b32 Pressed;
  b32 Clicked;
};

struct debug_text_render_group
{
  u32 SolidUIVertexBuffer;
  u32 SolidUIColorBuffer;
  u32 SolidUIUVBuffer;

  texture *FontTexture;
  shader Text2DShader;
  s32 TextTextureUniform;
  textured_2d_geometry_buffer Geo;

  shader DebugFontTextureShader;

  shader SolidUIShader;
};

