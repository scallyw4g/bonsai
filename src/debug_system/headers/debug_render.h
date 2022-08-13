
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

struct input;
struct debug_ui_render_group
{
  gpu_mapped_element_buffer*  GameGeo;
  shader*                     GameGeoShader;
  debug_text_render_group*    TextGroup;

  u64 InteractionStackTop;

  v2 *MouseP;
  v2 *MouseDP;
  v2 ScreenDim;
  input *Input;

  window_layout *HighestWindow; // NOTE(Jesse): Highest in terms of InteractionStackIndex

  umm HoverInteractionId;
  umm ClickedInteractionId;
  umm PressedInteractionId;

  untextured_2d_geometry_buffer Geo;

  ui_render_command_buffer *CommandBuffer;
};

